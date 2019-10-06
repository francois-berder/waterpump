/*
 * Copyright (C) 2019  Francois Berder <fberder@outlook.fr>
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

#include "app/board.h"
#include "mcu/timer.h"
#include "mcu/stm32l051xx.h"

struct timer_isr_ctx_t {
    timer_callback_t cb;
    void *arg;
};

#ifdef MCU_TIM2_EN
static struct timer_isr_ctx_t tim2_isr_ctx[TIMER_CHANNEL_COUNT];
#endif

#ifdef MCU_TIM6_EN
static struct timer_isr_ctx_t tim6_isr_ctx[TIMER_CHANNEL_COUNT];
#endif

#ifdef MCU_TIM21_EN
static struct timer_isr_ctx_t tim21_isr_ctx[TIMER_CHANNEL_COUNT];
#endif

#ifdef MCU_TIM22_EN
static struct timer_isr_ctx_t tim22_isr_ctx[TIMER_CHANNEL_COUNT];
#endif


static uint32_t get_pclk(TIM_TypeDef *dev)
{
    switch ((uintptr_t)dev) {
    case (uintptr_t)TIM21:
    case (uintptr_t)TIM22:
        return CLOCK_APB2;
    case (uintptr_t)TIM2:
    case (uintptr_t)TIM6:
        return CLOCK_APB1;
    default:
        return 0;
    }
}

static void timer_isr_handler(TIM_TypeDef *dev, struct timer_isr_ctx_t *callbacks)
{
    unsigned int i;
    for (i = 0; i < TIMER_CHANNEL_COUNT; i++) {
        if (dev->DIER & (TIM_DIER_CC1IE << i) && dev->SR & (TIM_SR_CC1IF << i)) {
            dev->SR &= ~(TIM_SR_CC1IF << i);
            if (callbacks[i].cb)
                callbacks[i].cb(callbacks[i].arg);
        }
    }
}

#ifdef MCU_TIM2_EN
void tim2_isr_handler(void)
{
    timer_isr_handler(TIM2, tim2_isr_ctx);
}
#endif

#ifdef MCU_TIM6_EN
void tim6_isr_handler(void)
{
    timer_isr_handler(TIM6, tim6_isr_ctx);
}
#endif

#ifdef MCU_TIM21_EN
void tim21_isr_handler(void)
{
    timer_isr_handler(TIM21, tim21_isr_ctx);
}
#endif

#ifdef MCU_TIM22_EN
void tim22_isr_handler(void)
{
    timer_isr_handler(TIM22, tim22_isr_ctx);
}
#endif

void timer_init(TIM_TypeDef *dev, uint32_t frequency, uint16_t res)
{
    dev->CR1 = 0;
    dev->CR2 = 0;
    dev->PSC = get_pclk(dev) / (frequency * res);
    dev->ARR = res;
    dev->EGR |= TIM_EGR_UG;
}

void timer_init_channel(TIM_TypeDef *dev, enum timer_channel_t channel, uint16_t counter, timer_callback_t cb, void *arg)
{
    if (counter > dev->ARR)
        counter = dev->ARR;

    switch (channel) {
    case TIMER_CHANNEL_1:
        dev->CCR1 = counter;
        break;
    case TIMER_CHANNEL_2:
        dev->CCR2 = counter;
        break;
    case TIMER_CHANNEL_3:
        dev->CCR3 = counter;
        break;
    case TIMER_CHANNEL_4:
        dev->CCR4 = counter;
        break;
    default:
        break;
    }

    switch ((uintptr_t)dev) {
#ifdef MCU_TIM2_EN
    case (uintptr_t)TIM2:
        tim2_isr_ctx[channel].cb = cb;
        tim2_isr_ctx[channel].arg = arg;
        break;
#endif
#ifdef MCU_TIM6_EN
    case (uintptr_t)TIM6:
        tim6_isr_ctx[channel].cb = cb;
        tim6_isr_ctx[channel].arg = arg;
        break;
#endif
#ifdef MCU_TIM21_EN
    case (uintptr_t)TIM21:
        tim21_isr_ctx[channel].cb = cb;
        tim21_isr_ctx[channel].arg = arg;
        break;
#endif
#ifdef MCU_TIM22_EN
    case (uintptr_t)TIM16:
        tim16_isr_ctx[channel].cb = cb;
        tim16_isr_ctx[channel].arg = arg;
        break;
#endif
    default:
        break;
    }

    dev->SR &= ~(TIM_SR_CC1IF << channel);
    dev->DIER |= TIM_DIER_CC1IE << channel;
}

void timer_deinit_channel(TIM_TypeDef *dev, enum timer_channel_t channel)
{
    dev->DIER &= ~(TIM_DIER_CC1IE << channel);
    dev->SR &= ~(TIM_SR_CC1IF << channel);
}

void timer_start(TIM_TypeDef *dev)
{
    dev->CR1 |= TIM_CR1_CEN;
}

void timer_stop(TIM_TypeDef *dev)
{
    dev->CR1 &= ~TIM_CR1_CEN;
}

uint32_t timer_get_frequency(TIM_TypeDef *dev)
{
    return get_pclk(dev) / ((dev->PSC + 1) * dev->ARR);
}

void timer_power_up(TIM_TypeDef *dev)
{
    switch ((uintptr_t)dev) {
#ifdef MCU_TIM2_EN
    case (uintptr_t)TIM2:
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
        break;
#endif
#ifdef MCU_TIM6_EN
    case (uintptr_t)TIM6:
        RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
        break;
#endif
#ifdef MCU_TIM21_EN
    case (uintptr_t)TIM21:
        RCC->APB2ENR |= RCC_APB2ENR_TIM21EN;
        break;
#endif
#ifdef MCU_TIM22_EN
    case (uintptr_t)TIM22:
        RCC->APB2ENR |= RCC_APB2ENR_TIM22EN;
        break;
#endif
    default:
        break;
    }
}

void timer_power_down(TIM_TypeDef *dev)
{
    timer_stop(dev);

    switch ((uintptr_t)dev) {
#ifdef MCU_TIM2_EN
    case (uintptr_t)TIM2:
        RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;
        break;
#endif
#ifdef MCU_TIM6_EN
    case (uintptr_t)TIM6:
        RCC->APB1ENR &= ~RCC_APB1ENR_TIM6EN;
        break;
#endif
#ifdef MCU_TIM21_EN
    case (uintptr_t)TIM21:
        RCC->APB2ENR &= ~RCC_APB2ENR_TIM21EN;
        break;
#endif
#ifdef MCU_TIM22_EN
    case (uintptr_t)TIM22:
        RCC->APB2ENR &= ~RCC_APB2ENR_TIM22EN;
        break;
#endif
    default:
        break;
    }
}
