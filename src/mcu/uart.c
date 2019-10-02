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
#include "mcu/uart.h"
#include "stm32l051xx.h"

#ifdef MCU_UART_EN

static inline uint32_t get_pclk(USART_TypeDef *dev)
{
    switch ((uintptr_t)dev) {
    case (uintptr_t)USART1:
        return CLOCK_APB2;
    case (uintptr_t)USART2:
        return CLOCK_APB1;
    default:
        return 0;
    }
}

void uart_init(USART_TypeDef *dev, unsigned int baudrate)
{
    if (baudrate)
        dev->BRR = get_pclk(dev) / baudrate;

    dev->CR1 = USART_CR1_TE | USART_CR1_RE;
    dev->CR2 = 0;
    dev->CR3 = 0;
}

void uart_enable(USART_TypeDef *dev)
{
    dev->CR1 |= USART_CR1_UE;
}

void uart_disable(USART_TypeDef *dev)
{
    dev->CR1 &= ~USART_CR1_UE;
}

void uart_power_up(USART_TypeDef *dev)
{
    switch ((uintptr_t)dev) {
    case (uintptr_t)USART1:
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        break;
    case (uintptr_t)USART2:
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
        break;
    }
}

void uart_power_down(USART_TypeDef *dev)
{
    switch ((uintptr_t)dev) {
    case (uintptr_t)USART1:
        RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
        break;
    case (uintptr_t)USART2:
        RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;
        break;
    }
}

void uart_send(USART_TypeDef *dev, const void *tx, uint32_t length)
{
    const uint8_t *buffer = (const uint8_t *)tx;

    if (!length)
        return;

    while (length--) {
        while (!(dev->ISR & USART_ISR_TXE));
        dev->TDR = *buffer++;
    }

    while (!(dev->ISR & USART_ISR_TC));
}

int uart_receive_noblock(USART_TypeDef *dev)
{
    uint8_t c;

    if (!(dev->ISR & USART_ISR_RXNE))
        return -1;

    c = (uint8_t)dev->RDR;
    return c;
}

#endif
