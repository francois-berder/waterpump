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

#ifndef PERIPH_UART_H
#define PERIPH_UART_H

#include "stm32l051xx.h"
#include <stdint.h>

/**
 * @brief Initialise UART peripheral
 *
 * The peripheral must be powered up before calling this function
 *
 * @param[in] dev
 * @param[in] baudrate
 */
void uart_init(USART_TypeDef *dev, unsigned int baudrate);

/**
 * @brief Enable UART peripheral
 *
 * @param[in] dev
 */
void uart_enable(USART_TypeDef *dev);

/**
 * @brief Disable UART peripheral
 *
 * @param[in] dev
 */
void uart_disable(USART_TypeDef *dev);

/**
 * @brief Power up UART peripheral
 *
 * @param[in] dev
 */
void uart_power_up(USART_TypeDef *dev);

/**
 * @brief Power down UART peripheral
 *
 * @param[in] dev
 */
void uart_power_down(USART_TypeDef *dev);

/**
 * @brief Send bytes over UART
 *
 * Note that this call is blocking.
 *
 * @param[in] dev
 * @param[in] tx
 * @param[in] length
 */
void uart_send(USART_TypeDef *dev, const void *tx, uint32_t length);

/**
 * @brief Receive one byte over UART
 *
 * This is a non-blocking call.
 *
 * @param[in] dev
 * @retval -1 No byte was available
 * @retval 0-0xFF A byte was read
 */
int uart_receive_noblock(USART_TypeDef *dev);

#endif
