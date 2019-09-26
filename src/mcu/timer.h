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

#ifndef PERIPH_TIMER_H
#define PERIPH_TIMER_H

#include "stm32l051xx.h"
#include <stdint.h>

enum timer_channel_t {
    TIMER_CHANNEL_1,
    TIMER_CHANNEL_2,
    TIMER_CHANNEL_3,
    TIMER_CHANNEL_4,
    TIMER_CHANNEL_COUNT,
};

typedef void(*timer_callback_t)(void*);

/**
 * @brief Initialize timer peripheral
 *
 * Timer peripheral must be powered up before calling this function.
 * Timer is not running after calling this function.
 *
 * @param[in] dev
 * @param[in] frequency
 * @param[in] res
 */
void timer_init(TIM_TypeDef *dev, uint32_t frequency, uint16_t res);

/**
 * @brief Initialise timer channel
 *
 * @param[in] dev
 * @param[in] channel
 * @param[in] counter
 * @param[in] cb
 * @param[in] arg
 */
void timer_init_channel(TIM_TypeDef *dev, enum timer_channel_t channel, uint16_t counter, timer_callback_t cb, void *arg);

/**
 * @brief De-initialise timer channel
 *
 * @param[in] dev
 * @param[in] channel
 */
void timer_deinit_channel(TIM_TypeDef *dev, enum timer_channel_t channel);

/**
 * @brief Start timer peripheral
 *
 * @param[in] dev
 */
void timer_start(TIM_TypeDef *dev);

/**
 * @brief Stop timer peripheral
 *
 * @param[in] dev
 */
void timer_stop(TIM_TypeDef *dev);

/**
 * @param[in] dev
 * @return Timer frequency in Hz
 */
uint32_t timer_get_frequency(TIM_TypeDef *dev);

/**
 * @brief Power up timer peripheral
 *
 * @param[in] dev
 */
void timer_power_up(TIM_TypeDef *dev);

/**
 * @brief Power down timer peripheral
 *
 * @param[in] dev
 */
void timer_power_down(TIM_TypeDef *dev);

#endif
