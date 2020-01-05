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

#include "pumps.h"
#include "schedule.h"
#include "mcu/eeprom.h"
#include "mcu/rtc.h"
#include <stdint.h>
#include <string.h>

#define SCHEDULE_MAGIC0     (0xae72d6f2)
#define SCHEDULE_MAGIC1     (0xc71f1cb3)

struct __attribute__((packed)) schedule_time_t {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    struct {
        uint8_t in_use : 1;
        uint8_t enable_pump1 : 1;
        uint8_t enable_pump2 : 1;
        uint8_t : 5;
    };
};

struct __attribute__((packed)) schedule_t {
    uint32_t magic0;
    struct schedule_time_t t0;
    struct schedule_time_t t1;
    uint32_t magic1;
};

/* Reserve some bytes in eeprom to store schedule */
static struct schedule_t __attribute__((used, section(".eeprom"))) schedule_info;

static int is_schedule_valid(struct schedule_t *s)
{
    return s->magic0 == SCHEDULE_MAGIC0 && s->magic1 == SCHEDULE_MAGIC1;
}

void handle_rtc_alarm_a(void)
{
    struct schedule_t s;

    eeprom_read((uint32_t)&schedule_info, &s, sizeof(s));
    if (!is_schedule_valid(&s))
        return;

    if (!s.t0.in_use)
        return;

    if (s.t0.enable_pump1 && s.t0.enable_pump2)
        pumps_start(PUMP_ALL);
    else if (s.t0.enable_pump1 && !s.t0.enable_pump2)
        pumps_start(PUMP_1);
    else if (!s.t0.enable_pump1 && s.t0.enable_pump2)
        pumps_start(PUMP_2);
}

void handle_rtc_alarm_b(void)
{
    struct schedule_t s;

    eeprom_read((uint32_t)&schedule_info, &s, sizeof(s));
    if (!is_schedule_valid(&s))
        return;

    if (!s.t1.in_use)
        return;

    if (s.t1.enable_pump1 && s.t1.enable_pump2)
        pumps_start(PUMP_ALL);
    else if (s.t1.enable_pump1 && !s.t1.enable_pump2)
        pumps_start(PUMP_1);
    else if (!s.t1.enable_pump1 && s.t1.enable_pump2)
        pumps_start(PUMP_2);
}

void schedule_init(void)
{
    struct schedule_t s;

    eeprom_read((uint32_t)&schedule_info, &s, sizeof(s));

    if (!is_schedule_valid(&s)) {
        memset(&s, 0, sizeof(s));
        s.magic0 = SCHEDULE_MAGIC0;
        s.magic1 = SCHEDULE_MAGIC1;
        eeprom_write((uint32_t)&schedule_info, &s, sizeof(s));
        rtc_remove_alarm(RTC_ALARM_A);
        rtc_remove_alarm(RTC_ALARM_B);
    } else {
        /* Configure alarm for schedule 0 */
        if (s.t0.in_use)
            rtc_set_alarm(RTC_ALARM_A, s.t0.hour, s.t0.min, s.t0.sec, handle_rtc_alarm_a);
        else
            rtc_remove_alarm(RTC_ALARM_A);

        /* Configure alarm for schedule 1 */
        if (s.t1.in_use)
            rtc_set_alarm(RTC_ALARM_B, s.t1.hour, s.t1.min, s.t1.sec, handle_rtc_alarm_b);
        else
            rtc_remove_alarm(RTC_ALARM_B);
    }
}

void schedule_configure(int index, uint8_t hour, uint8_t min, uint8_t sec, enum pump_t pumps)
{
    struct schedule_t s;

    /* Store new schedule in EEPROM */
    eeprom_read((uint32_t)&schedule_info, &s, sizeof(s));
    if (!is_schedule_valid(&s))
        return;

    if (index == 0) {
        s.t0.hour = hour;
        s.t0.min = min;
        s.t0.sec = sec;
        if (pumps == PUMP_ALL || pumps == PUMP_1)
            s.t0.enable_pump1 = 1;
        if (pumps == PUMP_ALL || pumps == PUMP_2)
            s.t0.enable_pump2 = 1;
    } else if (index == 1) {
        s.t1.hour = hour;
        s.t1.min = min;
        s.t1.sec = sec;
        if (pumps == PUMP_ALL || pumps == PUMP_1)
            s.t1.enable_pump1 = 1;
        if (pumps == PUMP_ALL || pumps == PUMP_2)
            s.t1.enable_pump2 = 1;
    }
    eeprom_write((uint32_t)&schedule_info, &s, sizeof(s));

    schedule_enable(index);
}

void schedule_enable(int index)
{
    struct schedule_t s;

    /* Store new schedule in EEPROM */
    eeprom_read((uint32_t)&schedule_info, &s, sizeof(s));
    if (!is_schedule_valid(&s))
        return;

    if (index == 0)
        s.t0.in_use = 1;
    else if (index == 1)
        s.t1.in_use = 1;
    eeprom_write((uint32_t)&schedule_info, &s, sizeof(s));

    /* Enable alarm */
    if (index == 0)
        rtc_set_alarm(RTC_ALARM_A, s.t0.hour, s.t0.min, s.t0.sec, handle_rtc_alarm_a);
    else if (index == 1)
        rtc_set_alarm(RTC_ALARM_B, s.t1.hour, s.t1.min, s.t1.sec, handle_rtc_alarm_b);
}

void schedule_disable(int index)
{
    struct schedule_t s;

    /* Store new schedule in EEPROM */
    eeprom_read((uint32_t)&schedule_info, &s, sizeof(s));
    if (!is_schedule_valid(&s))
        return;

    if (index == 0)
        s.t0.in_use = 0;
    else if (index == 1)
        s.t1.in_use = 0;
    eeprom_write((uint32_t)&schedule_info, &s, sizeof(s));

    /* Disable alarm */
    if (index == 0)
        rtc_remove_alarm(RTC_ALARM_A);
    else if (index == 1)
        rtc_remove_alarm(RTC_ALARM_B);
}

void schedule_to_string(char *dst)
{
    struct schedule_t s;

    dst[0] = '\0';
    eeprom_read((uint32_t)&schedule_info, &s, sizeof(s));

    if (!is_schedule_valid(&s)) {
        strcpy(dst, "SCHEDULE INVALID\r\n");
    } else {
        char *tmp;
        /* Schedule 0 */
        strcat(dst, "0: ");
        if (s.t0.in_use)
            strcat(dst, "ON ");
        else
            strcat(dst, "OFF ");

        if (s.t0.enable_pump1 && s.t0.enable_pump2)
            strcat(dst, "PUMP 1 & 2 ");
        if (s.t0.enable_pump1 && !s.t0.enable_pump2)
            strcat(dst, "PUMP 1 ");
        if (!s.t0.enable_pump1 && s.t0.enable_pump2)
            strcat(dst, "PUMP 2 ");

        tmp = &dst[strlen(dst)];
        *tmp++ = '0' + s.t0.hour / 10;
        *tmp++ = '0' + s.t0.hour % 10;
        *tmp++ = ':';
        *tmp++ = '0' + s.t0.min / 10;
        *tmp++ = '0' + s.t0.min % 10;
        *tmp++ = ':';
        *tmp++ = '0' + s.t0.sec / 10;
        *tmp++ = '0' + s.t0.sec % 10;
        *tmp++ = '\0';

        strcat(dst, "\r\n");
        strcat(dst, "\r\n");

        /* Schedule 1 */
        strcat(dst, "1: ");
        if (s.t1.in_use)
            strcat(dst, "ON ");
        else
            strcat(dst, "OFF ");

        if (s.t1.enable_pump1 && s.t1.enable_pump2)
            strcat(dst, "PUMP 1 & 2 ");
        if (s.t1.enable_pump1 && !s.t1.enable_pump2)
            strcat(dst, "PUMP 1 ");
        if (!s.t1.enable_pump1 && s.t1.enable_pump2)
            strcat(dst, "PUMP 2 ");

        tmp = &dst[strlen(dst)];
        *tmp++ = '0' + s.t1.hour / 10;
        *tmp++ = '0' + s.t1.hour % 10;
        *tmp++ = ':';
        *tmp++ = '0' + s.t1.min / 10;
        *tmp++ = '0' + s.t1.min % 10;
        *tmp++ = ':';
        *tmp++ = '0' + s.t1.sec / 10;
        *tmp++ = '0' + s.t1.sec % 10;
        *tmp++ = '\0';

        strcat(dst, "\r\n");
    }
}
