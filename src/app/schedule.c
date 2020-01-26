/*
 * Copyright (C) 2019 Francois Berder <fberder@outlook.fr>
 *
 * This file is part of waterpump.
 *
 * waterpump is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * waterpump is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with waterpump.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "board.h"
#include "pumps.h"
#include "schedule.h"
#include "sim800l.h"
#include "mcu/eeprom.h"
#include <stdint.h>
#include <string.h>

#define SCHEDULE_MAGIC0     (0xae72d6f2)
#define SCHEDULE_MAGIC1     (0xc71f1cb3)

struct __attribute__((packed)) schedule_time_t {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    struct {
        uint8_t enabled : 1;
        uint8_t enable_pump1 : 1;
        uint8_t enable_pump2 : 1;
        uint8_t duration : 5;
    };
};

struct __attribute__((packed)) schedule_t {
    uint32_t magic0;
    struct schedule_time_t alarms[SIM800L_ALARM_COUNT];
    uint32_t magic1;
};

/* Reserve some bytes in eeprom to store schedule */
static struct schedule_t __attribute__((used, section(".eeprom"))) schedule;

static struct sim800l_params_t gsm_params = {
    .dev = SIM800_USART,
};

static int is_schedule_valid(struct schedule_t *s)
{
    return s->magic0 == SCHEDULE_MAGIC0 && s->magic1 == SCHEDULE_MAGIC1;
}

void alarm_callback(uint8_t index)
{
    struct schedule_t s;

    eeprom_read((uint32_t)&schedule, &s, sizeof(s));
    if (!is_schedule_valid(&s))
        return;

    if (!schedule.alarms[index].enabled || !schedule.alarms[index].duration)
        return;

    if (schedule.alarms[index].enable_pump1 && !schedule.alarms[index].enable_pump2)
        pumps_start(PUMP_1, schedule.alarms[index].duration);
    if (!schedule.alarms[index].enable_pump1 && schedule.alarms[index].enable_pump2)
        pumps_start(PUMP_2, schedule.alarms[index].duration);
    if (schedule.alarms[index].enable_pump1 && schedule.alarms[index].enable_pump2)
        pumps_start(PUMP_ALL, schedule.alarms[index].duration);
}

void schedule_init(void)
{
    int i;
    struct schedule_t s;

    /* Delete all existing alarms on SIM800L */
    for (i = 0; i < SIM800L_ALARM_COUNT; ++i)
        sim800l_delete_alarm(&gsm_params, 1 + i);

    eeprom_read((uint32_t)&schedule, &s, sizeof(s));

    if (!is_schedule_valid(&s)) {
        memset(&s, 0, sizeof(s));
        s.magic0 = SCHEDULE_MAGIC0;
        s.magic1 = SCHEDULE_MAGIC1;
        eeprom_write((uint32_t)&schedule, &s, sizeof(s));
    } else {
        for (i = 0; i < SIM800L_ALARM_COUNT; ++i) {
            if (schedule.alarms[i].enabled)
                schedule_enable(i);
        }
    }
}

void schedule_configure(int index, uint8_t hour, uint8_t min, uint8_t sec, enum pump_t pumps, uint8_t duration)
{
    struct schedule_t s;

    if (index < SCHEDULE_COUNT)
        return;

    /* Store new schedule in EEPROM */
    eeprom_read((uint32_t)&schedule, &s, sizeof(s));
    if (!is_schedule_valid(&s))
        return;

    schedule.alarms[index].hour = hour;
    schedule.alarms[index].min = min;
    schedule.alarms[index].sec = sec;
    schedule.alarms[index].enable_pump1 = (pumps & PUMP_1);
    schedule.alarms[index].enable_pump2 = (pumps & PUMP_2);
    schedule.alarms[index].duration = duration;
    eeprom_write((uint32_t)&schedule, &s, sizeof(s));

    schedule_enable(index);
}

void schedule_enable(int index)
{
    struct schedule_t s;
    uint8_t hour, min, sec;

    if (index < SCHEDULE_COUNT)
        return;

    /* Store new schedule in EEPROM */
    eeprom_read((uint32_t)&schedule, &s, sizeof(s));
    if (!is_schedule_valid(&s))
        return;
    schedule.alarms[index].enabled = 1;
    eeprom_write((uint32_t)&schedule, &s, sizeof(s));

    sim800l_delete_alarm(&gsm_params, 1 + index);

    /* Convert hour, min, sec to BCD format */
    hour = (schedule.alarms[index].hour / 10) << 4;
    hour |= schedule.alarms[index].hour % 10;
    min = (schedule.alarms[index].min / 10) << 4;
    min |= schedule.alarms[index].min % 10;
    sec = (schedule.alarms[index].sec / 10) << 4;
    sec |= schedule.alarms[index].sec % 10;

    sim800l_set_alarm(&gsm_params, 1 + index, hour, min, sec, alarm_callback);
}

void schedule_disable(int index)
{
    struct schedule_t s;

    if (index < SCHEDULE_COUNT)
        return;

    /* Store new schedule in EEPROM */
    eeprom_read((uint32_t)&schedule, &s, sizeof(s));
    if (!is_schedule_valid(&s))
        return;
    schedule.alarms[index].enabled = 0;
    eeprom_write((uint32_t)&schedule, &s, sizeof(s));

    sim800l_delete_alarm(&gsm_params, 1 + index);
}

void schedule_to_string(char *dst)
{
    struct schedule_t s;
    int i;

    dst[0] = '\0';
    eeprom_read((uint32_t)&schedule, &s, sizeof(s));

    if (!is_schedule_valid(&s)) {
        strcpy(dst, "SCHEDULE INVALID\r\n");
        return;
    }

    for (i = 0; i < SIM800L_ALARM_COUNT; ++i) {
        char *tmp = dst;
        *tmp++ = '0' + i;
        *tmp++ = ':';
        *tmp++ = ' ';
        *tmp++ = '\0';

        if (schedule.alarms[i].enabled)
            strcat(dst, "ON ");
        else
            strcat(dst, "OFF ");

        if (schedule.alarms[i].enable_pump1 && schedule.alarms[i].enable_pump2)
            strcat(dst, "PUMP 1 & 2 ");
        else if (schedule.alarms[i].enable_pump1 && !schedule.alarms[i].enable_pump2)
            strcat(dst, "PUMP 1 ");
        else if (!schedule.alarms[i].enable_pump1 && schedule.alarms[i].enable_pump2)
            strcat(dst, "PUMP 2 ");

        tmp = &dst[strlen(dst)];
        *tmp++ = '0' + schedule.alarms[i].hour / 10;
        *tmp++ = '0' + schedule.alarms[i].hour % 10;
        *tmp++ = ':';
        *tmp++ = '0' + schedule.alarms[i].min / 10;
        *tmp++ = '0' + schedule.alarms[i].min % 10;
        *tmp++ = ':';
        *tmp++ = '0' + schedule.alarms[i].sec / 10;
        *tmp++ = '0' + schedule.alarms[i].sec % 10;
        *tmp++ = ' ';

        if (schedule.alarms[i].duration >= 10)
            *tmp++ = '0' + schedule.alarms[i].duration / 10;
        *tmp++ = '0' + schedule.alarms[i].duration % 10;
        *tmp++ = '\0';

        strcat(dst, "\r\n");
    }
}
