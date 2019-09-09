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

#ifndef PERIPH_GPIO_H
#define PERIPH_GPIO_H

#include <stdint.h>

#define GPIO_PIN(PORT,INDEX)    ((((PORT) & 0xF) << 4) | ((INDEX) & 0xF))
#define INVALID_GPIO            (0xFF)

enum pull_mode_t {
    PULL_NONE,
    PULL_DOWN,
    PULL_UP,
};

enum gpio_flank_t {
    GPIO_RISING = 1,
    GPIO_FALLING = 2,
    GPIO_EDGE = 3
};

enum gpio_port_t {
    PORT_A,
    PORT_B,
    PORT_C,
    PORT_D,
    PORT_H,
    PORT_COUNT,
};

typedef void(*gpio_callback_t)(void *);

/**
 * @brief Configure GPIO as an output
 *
 * @param[in] gpio
 * @param[in] val
 */
void gpio_init_out(uint8_t gpio, int val);

/**
 * @brief Configure GPIO as an output drain
 *
 * @param[in] gpio
 * @param[in] mode
 * @param[in] val
 */
void gpio_init_od(uint8_t gpio, enum pull_mode_t mode, int val);

/**
 * @brief Configure GPIO as an input
 *
 * @param[in] gpio
 * @param[in] mode
 */
void gpio_init_in(uint8_t gpio, enum pull_mode_t mode);

/**
 * @brief Configure GPIO as alternate function
 *
 * @param[in] gpio
 * @param[in] function 4-bit value
 */
void gpio_init_af(uint8_t gpio, uint8_t function);

/**
 * @brief Configure GPIO as analog
 *
 * @param[in] gpio
 */
void gpio_init_analog(uint8_t gpio);

/**
 * @brief Configure GPIO as interrupt pin
 *
 * @param[in] gpio
 * @param[in] mode
 * @param[in] flank
 */
void gpio_init_int(uint8_t gpio, enum pull_mode_t mode, enum gpio_flank_t flank);

/**
 * @brief Set GPIO value
 *
 * @param[in] gpio
 * @param[in] val 0: set to low, otherwise set to high
 */
void gpio_write(uint8_t gpio, int val);

/**
 * @brief Toggle GPIO
 *
 * @param[in] gpio
 */
void gpio_toggle(uint8_t gpio);

/**
 * @brief Read GPIO value
 *
 * @param[in] gpio
 * @retval 0 Input is low
 * @retval 1 Input is high
 */
int gpio_read(uint8_t gpio);

#endif
