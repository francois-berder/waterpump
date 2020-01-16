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

static rtc_alarm_cb_t callbacks[RTC_ALARM_COUNT];

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

void rtc_isr_handler(void)
{
    if (RTC->ISR & RTC_ISR_ALRAF) {
        if (callbacks[RTC_ALARM_A])
            callbacks[RTC_ALARM_A]();
        RTC->ISR &= ~RTC_ISR_ALRAF;
    } else if (RTC->ISR & RTC_ISR_ALRBF) {
        if (callbacks[RTC_ALARM_B])
            callbacks[RTC_ALARM_B]();
        RTC->ISR &= ~RTC_ISR_ALRBF;
    }

    EXTI->PR |= EXTI_PR_PR17;
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

void rtc_set_prescaler(uint8_t a, uint16_t s)
{
    rtc_unlock();
    RTC->PRER = ((a << RTC_PRER_PREDIV_A_Pos) & RTC_PRER_PREDIV_A)
              | ((s << RTC_PRER_PREDIV_S_Pos) & RTC_PRER_PREDIV_S);
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

void rtc_get_time(union rtc_time_t *t)
{
    rtc_unlock();
    t->asUint64 = (RTC->DR & 0xFF1F3F) | (RTC->TR & 0x7F7F7F);
    rtc_lock();
}

void rtc_set_alarm(enum rtc_alarm_t index, uint8_t hour, uint8_t min, uint8_t sec, rtc_alarm_cb_t cb)
{
    if (index >= RTC_ALARM_COUNT)
        return;

    rtc_unlock();
    if (index == RTC_ALARM_A) {
        RTC->CR &=~ RTC_CR_ALRAE;
        while((RTC->ISR & RTC_ISR_ALRAWF) != RTC_ISR_ALRAWF);
        callbacks[RTC_ALARM_A] = cb;
        RTC->ALRMAR = RTC_ALRMAR_MSK4;
        RTC->ALRMAR |= ((hour / 10) << 20) | ((hour % 10) << 16);
        RTC->ALRMAR |= ((min / 10) << 12) | ((min % 10) << 8);
        RTC->ALRMAR |= ((sec / 10) << 4) | (sec % 10);
        RTC->CR = RTC_CR_ALRAIE | RTC_CR_ALRAE;
    } else if (index == RTC_ALARM_B) {
        RTC->CR &=~ RTC_CR_ALRBE;
        while((RTC->ISR & RTC_ISR_ALRBWF) != RTC_ISR_ALRBWF);
        callbacks[RTC_ALARM_B] = cb;
        RTC->ALRMBR = RTC_ALRMBR_MSK4;
        RTC->ALRMBR |= ((hour / 10) << 20) | ((hour % 10) << 16);
        RTC->ALRMBR |= ((min / 10) << 12) | ((min % 10) << 8);
        RTC->ALRMBR |= ((sec / 10) << 4) | (sec % 10);
        RTC->CR = RTC_CR_ALRBIE | RTC_CR_ALRBE;
    }
    rtc_lock();

    /* Configure EXTI line (necessary to trigger interrupt using alarms) */
    EXTI->FTSR &= ~EXTI_FTSR_TR17;
    EXTI->RTSR |= EXTI_RTSR_TR17;
    EXTI->IMR  |= EXTI_IMR_IM17;
    EXTI->PR   |= EXTI_PR_PR17;
}

void rtc_remove_alarm(enum rtc_alarm_t index)
{
    rtc_unlock();
    if (index == RTC_ALARM_A) {
        RTC->CR &=~ RTC_CR_ALRAE;
        while((RTC->ISR & RTC_ISR_ALRAWF) != RTC_ISR_ALRAWF);
    } else if (index == RTC_ALARM_B) {
        RTC->CR &=~ RTC_CR_ALRBE;
        while((RTC->ISR & RTC_ISR_ALRBWF) != RTC_ISR_ALRBWF);
    }
    rtc_lock();
}
