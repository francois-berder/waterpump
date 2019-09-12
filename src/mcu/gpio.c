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
#include "mcu/stm32l051xx.h"
#include <stdint.h>

#define GPIO_PORT(PIN)      (((PIN) >> 4) & 0xF)
#define GPIO_IDX(PIN)       ((PIN) & 0xF)

static GPIO_TypeDef* const base_address[PORT_COUNT] = {
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOH,
};

static void dummy_ext_callback(void)
{

}

void __attribute__((weak, alias("dummy_ext_callback"))) ext0_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext1_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext2_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext3_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext4_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext5_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext6_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext7_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext8_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext9_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext10_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext11_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext12_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext13_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext14_callback(void);
void __attribute__((weak, alias("dummy_ext_callback"))) ext15_callback(void);

void exti0_1_isr_handler(void)
{
    if (EXTI->PR & (1U << 0)) { ext0_callback(); EXTI->PR |= 1U << 0; }
    if (EXTI->PR & (1U << 1)) { ext1_callback(); EXTI->PR |= 1U << 1; }
}

void exti2_3_isr_handler(void)
{
    if (EXTI->PR & (1U << 2)) { ext2_callback(); EXTI->PR |= 1U << 2; }
    if (EXTI->PR & (1U << 3)) { ext3_callback(); EXTI->PR |= 1U << 3; }
}

void exti4_15_isr_handler(void)
{
    if (EXTI->PR & (1U << 4)) { ext4_callback(); EXTI->PR |= 1U << 4; }
    if (EXTI->PR & (1U << 5)) { ext5_callback(); EXTI->PR |= 1U << 5; }
    if (EXTI->PR & (1U << 6)) { ext6_callback(); EXTI->PR |= 1U << 6; }
    if (EXTI->PR & (1U << 7)) { ext7_callback(); EXTI->PR |= 1U << 7; }
    if (EXTI->PR & (1U << 8)) { ext8_callback(); EXTI->PR |= 1U << 8; }
    if (EXTI->PR & (1U << 9)) { ext9_callback(); EXTI->PR |= 1U << 9; }
    if (EXTI->PR & (1U << 10)) { ext10_callback(); EXTI->PR |= 1U << 10; }
    if (EXTI->PR & (1U << 11)) { ext11_callback(); EXTI->PR |= 1U << 11; }
    if (EXTI->PR & (1U << 12)) { ext12_callback(); EXTI->PR |= 1U << 12; }
    if (EXTI->PR & (1U << 13)) { ext13_callback(); EXTI->PR |= 1U << 13; }
    if (EXTI->PR & (1U << 14)) { ext14_callback(); EXTI->PR |= 1U << 14; }
    if (EXTI->PR & (1U << 15)) { ext15_callback(); EXTI->PR |= 1U << 15; }
}

static void configure_pull_up_down(GPIO_TypeDef * const regs, uint32_t index, enum pull_mode_t mode)
{
    regs->PUPDR &= ~(0x3U << (2 * index));
    switch (mode) {
    case PULL_UP:
        regs->PUPDR |= 0x1U << (2 * index);
        break;
    case PULL_DOWN:
        regs->PUPDR |= 0x2U << (2 * index);
        break;
    default:
        break;
    }
}

void gpio_init_out(uint8_t gpio, int val)
{
    uint32_t port = GPIO_PORT(gpio);
    uint32_t index = GPIO_IDX(gpio);
    GPIO_TypeDef * const regs = base_address[port];

    RCC->AHBENR |= RCC_IOPENR_GPIOAEN << port;

    configure_pull_up_down(regs, index, PULL_NONE);
    if (val)
        regs->BSRR |= 0x1U << index;
    else
        regs->BRR |= 0x1U << index;

    regs->MODER &= ~(0x3U << (2 * index));
    regs->OTYPER &= ~(0x1U << index);
    regs->OSPEEDR &= ~(0x3U << (2 * index));    /* slow speed */
    regs->MODER |= 0x1U << (2 * index);
}

void gpio_init_od(uint8_t gpio, enum pull_mode_t mode, int val)
{
    uint32_t port = GPIO_PORT(gpio);
    uint32_t index = GPIO_IDX(gpio);
    GPIO_TypeDef * const regs = base_address[port];

    RCC->AHBENR |= RCC_IOPENR_GPIOAEN << port;

    configure_pull_up_down(regs, index, mode);
    if (val)
        regs->BSRR |= 0x1U << index;
    else
        regs->BRR |= 0x1U << index;

    regs->MODER &= ~(0x3U << (2 * index));
    regs->OTYPER &= ~(0x1U << index);
    regs->OSPEEDR &= ~(0x3U << (2 * index));    /* slow speed */
    regs->OTYPER |= 0x1U << index;
    regs->MODER |= 0x1U << (2 * index);
}

void gpio_init_in(uint8_t gpio, enum pull_mode_t mode)
{
    uint32_t port = GPIO_PORT(gpio);
    uint32_t index = GPIO_IDX(gpio);
    GPIO_TypeDef * const regs = base_address[port];

    RCC->AHBENR |= RCC_IOPENR_GPIOAEN << port;

    configure_pull_up_down(regs, index, mode);

    regs->MODER &= ~(0x3U << (2 * index));
}

void gpio_init_af(uint8_t gpio, uint8_t function)
{
    uint32_t port = GPIO_PORT(gpio);
    uint32_t index = GPIO_IDX(gpio);
    GPIO_TypeDef * const regs = base_address[port];

    RCC->AHBENR |= RCC_IOPENR_GPIOAEN << port;

    function &= 0xF;
    if (index < 8) {
        regs->AFR[0] &= ~(0xFU << (4 * index));
        regs->AFR[0] |= function << (4 * index);
    } else {
        regs->AFR[1] &= ~(0xFU << (4 * (index - 8)));
        regs->AFR[1] |= function << (4 * (index - 8));
    }

    regs->OTYPER &= ~(0x1U << index);           /* push-pull */
    regs->OSPEEDR |= 0x3U << (2 * index);       /* high speed */
    regs->PUPDR &= ~(0x3U << (2 * index));      /* No pull-down/pull-up */
    regs->MODER &= ~(0x3U << (2 * index));      /* AF */
    regs->MODER |= 0x2U << (2 * index);
}

void gpio_init_analog(uint8_t gpio)
{
    uint32_t port = GPIO_PORT(gpio);
    uint32_t index = GPIO_IDX(gpio);
    GPIO_TypeDef * const regs = base_address[port];

    RCC->AHBENR |= RCC_IOPENR_GPIOAEN << port;

    regs->MODER |= 0x3U << (2 * index);
}

void gpio_init_int(uint8_t gpio, enum pull_mode_t mode, enum gpio_flank_t flank)
{
    uint32_t port = GPIO_PORT(gpio);
    uint32_t index = GPIO_IDX(gpio);

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    gpio_init_in(gpio, mode);

    EXTI->RTSR &= ~(0x1U << index);
    EXTI->RTSR |= (flank & 0x1U) << index;
    EXTI->FTSR &= ~(0x1U << index);
    EXTI->FTSR |= (flank >> 1) << index;

    SYSCFG->EXTICR[index >> 2] &= ~(0xFU << ((index & 0x03U) * 4));
    SYSCFG->EXTICR[index >> 2] |= (port << ((index & 0x3U) * 4));

    EXTI->PR |= 0x1U << index;
    EXTI->IMR |= 0x1U << index;
}

void gpio_write(uint8_t gpio, int val)
{
    uint32_t port = GPIO_PORT(gpio);
    uint32_t index = GPIO_IDX(gpio);
    GPIO_TypeDef * const regs = base_address[port];

    RCC->AHBENR |= RCC_IOPENR_GPIOAEN << port;

    if (val)
        regs->BSRR |= 1U << index;
    else
        regs->BRR |= 1U << index;
}

void gpio_toggle(uint8_t gpio)
{
    uint32_t port = GPIO_PORT(gpio);
    uint32_t index = GPIO_IDX(gpio);
    GPIO_TypeDef * const regs = base_address[port];

    RCC->AHBENR |= RCC_IOPENR_GPIOAEN << port;

    regs->ODR ^= 1U << index;
}

int gpio_read(uint8_t gpio)
{
    uint32_t port = GPIO_PORT(gpio);
    uint32_t index = GPIO_IDX(gpio);
    GPIO_TypeDef * const regs = base_address[port];

    RCC->AHBENR |= RCC_IOPENR_GPIOAEN << port;

    return !!(regs->IDR & (1U << index));
}
