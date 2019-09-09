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

#include "mcu/gpio.h"

#define ENABLE_GSM_PIN      (GPIO_PIN(PORT_B, 7))
#define ENABLE_PUMP1_PIN    (GPIO_PIN(PORT_B, 6))
#define ENABLE_PUMP2_PIN    (GPIO_PIN(PORT_B, 5))
#define LED_PIN             (GPIO_PIN(PORT_A, 7))
#define REQ_WATER_PIN       (GPIO_PIN(PORT_A, 5))

int main(void)
{
    /* 1. Initialize GPIOs */
    gpio_init_out(LED_PIN, 1);
    gpio_init_out(ENABLE_GSM_PIN, 0);
    gpio_init_out(ENABLE_PUMP1_PIN, 0);
    gpio_init_out(ENABLE_PUMP2_PIN, 0);
    gpio_init_int(REQ_WATER_PIN, PULL_NONE, GPIO_FALLING);

    while (1);

    __builtin_unreachable();
}
