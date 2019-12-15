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

#ifndef RTC_H
#define RTC_H

#include <stdint.h>

union rtc_time_t {
    struct {
        /* All elements are in BCD */
        uint8_t sec;
        uint8_t min;
        uint8_t hour;
        uint8_t day;
        uint8_t month;
        uint8_t year;
        uint8_t : 8;
        uint8_t : 8;
    };
    uint64_t asUint64;
};

enum rtc_alarm_t {
    RTC_ALARM_A,
    RTC_ALARM_B,
    RTC_ALARM_COUNT,
};

typedef void(*rtc_alarm_cb_t)(void);

/**
 * @brief Initialize RTC module
 */
void rtc_init(void);

/**
 * @brief Set time
 *
 * @param[in] t
 */
void rtc_set_time(union rtc_time_t t);

/**
 * @brief Set alarm
 *
 * @param[in] index
 * @param[in] hour in BCD
 * @param[in] min in BCD
 * @param[in] sec in BCD
 * @param[in] cb
 */
void rtc_set_alarm(enum rtc_alarm_t index, uint8_t hour, uint8_t min, uint8_t sec, rtc_alarm_cb_t cb);

/**
 * @brief Remove alarm
 *
 * @param[in] index
 */
void rtc_remove_alarm(enum rtc_alarm_t index);

#endif
