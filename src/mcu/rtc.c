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

    RTC->ISR |= RTC_ISR_INIT;
    while (!(RTC->ISR & RTC_ISR_INITF));
}

static void rtc_lock(void)
{
    RTC->ISR &= ~RTC_ISR_INIT;
    while (RTC->ISR & RTC_ISR_INITF);
    RTC->WPR = 0xFF;
}

void rtc_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_DBP;
    /* Clock RTC using LSI */
    RCC->CSR &= ~RCC_CSR_RTCSEL;
    RCC->CSR |= RCC_CSR_RTCSEL_LSI;
    RCC->CSR |= RCC_CSR_RTCEN;

    rtc_unlock();
    RTC->CR = 0;
    RTC->TR = 0;
    RTC->DR = 0;
    rtc_lock();
}

void rtc_set_time(union rtc_time_t t)
{
    rtc_unlock();
    RTC->TR = t.asUint64 & 0xFFFFFF;    /* Set hour, minute and second */
    /* Set year, month, date. Week day set to Monday */
    RTC->DR = ((t.asUint64 >> 24) & 0x1FFF) | (0x1 << RTC_DR_WDU_Pos) | (t.year << RTC_DR_YU_Pos);
    rtc_lock();
    while (!(RTC->ISR & RTC_ISR_RSF));
}
