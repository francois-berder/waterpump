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

#ifndef BOARD_H
#define BOARD_H

#include "app/sim800l.h"
#include "mcu/gpio.h"

#define ENABLE_GSM_PIN      (GPIO_PIN(PORT_A, 0))
#define ENABLE_PUMP1_PIN    (GPIO_PIN(PORT_B, 6))
#define ENABLE_PUMP2_PIN    (GPIO_PIN(PORT_B, 5))
#define LED_PIN             (GPIO_PIN(PORT_A, 7))
#define REQ_WATER_PIN       (GPIO_PIN(PORT_A, 5))
#define GSM_TX_PIN          (GPIO_PIN(PORT_A, 2))
#define GSM_RX_PIN          (GPIO_PIN(PORT_A, 3))

#define CORE_CLOCK          (4190000)
#define CLOCK_APB1          (CORE_CLOCK)
#define CLOCK_APB2          (CORE_CLOCK)

#define MCU_TIM2_EN     /* Used by pumps module */
#define MCU_TIM21_EN    /* Used by SIM800 module */
#define MCU_TIM22_EN    /* Used by status module */
#define MCU_UART_EN

#define SIM800_USART        (USART2)

#endif
