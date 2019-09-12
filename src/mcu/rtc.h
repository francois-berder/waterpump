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

#ifndef PERIPH_RTC_H
#define PERIPH_RTC_H

#include <stdint.h>

enum rtc_wkup_prescaler_t {
    RTC_WKUP_PRE_16,
    RTC_WKUP_PRE_8,
    RTC_WKUP_PRE_4,
    RTC_WKUP_PRE_2,
};

/**
 * @brief Start wake up timer
 *
 * RTC wake up interrupt is enabled so that the wake up timer
 * can wake up the device from sleep, standby, stop mode.
 *
 * @param[in] prescaler
 * @param[in] counter
 */
void rtc_start_wakeup_timer(enum rtc_wkup_prescaler_t prescaler, uint16_t counter);

/**
 * @brief Stop wake up timer
 */
void rtc_stop_wakeup_timer(void);

#endif
