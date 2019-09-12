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

#include "mcu/rtc.h"
#include "mcu/stm32l051xx.h"
#include <stdint.h>

static void rtc_unlock(void)
{
    /* Unlock RTC registers write protection */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
}

static void rtc_lock(void)
{
    RTC->WPR = 0xFF;
}

void rtc_isr_handler(void)
{
    RTC->ISR &= ~RTC_ISR_WUTF;
}

void rtc_start_wakeup_timer(enum rtc_wkup_prescaler_t prescaler, uint16_t counter)
{
    rtc_unlock();

    /* Stop wake up timer and wait until it is safe to configure it */
    RTC->CR &= ~RTC_CR_WUTE;
    while (!(RTC->ISR & RTC_ISR_WUTWF));

    RTC->CR &= ~RTC_CR_WUCKSEL;
    RTC->CR |= prescaler & RTC_CR_WUCKSEL;
    RTC->WUTR = counter;
    RTC->CR |= RTC_CR_WUTIE; /* Enable interrupt to wake up device */
    RTC->CR |= RTC_CR_WUTE;

    rtc_lock();
}

void rtc_stop_wakeup_timer(void)
{
    rtc_unlock();
    RTC->CR &= ~RTC_CR_WUTE;
    rtc_lock();
}
