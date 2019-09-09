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

#include "stm32l051xx.h"
#include <stdint.h>

extern uint32_t _end_stack;
extern uint32_t _end_text;
extern uint32_t _start_data;
extern uint32_t _end_data;
extern uint32_t _start_bss;
extern uint32_t _end_bss;

int main(void);

void reset_handler(void)
{
    register uint32_t *src, *dst;

    /* Copy data section from flash to RAM */
    src = &_end_text;
    dst = &_start_data;
    while (dst < &_end_data)
        *dst++ = *src++;

    /* Clear the bss section, assumes .bss goes directly after .data */
    dst = &_start_bss;
    while (dst < &_end_bss)
        *dst++ = 0;

    main();

    __disable_irq();
    while (1);
}

void default_handler(void)
{
    __disable_irq();
    while (1);
}

void nmi_handler(void) __attribute__((weak, alias("default_handler")));
void hardfault_handler(void) __attribute__((weak, alias("default_handler")));
void svcall_handler(void) __attribute__((weak, alias("default_handler")));
void pendsv_handler(void) __attribute__((weak, alias("default_handler")));
void systick_handler(void) __attribute__((weak, alias("default_handler")));

void * const core_vector_table[16] __attribute__ ((section(".cortex_vectors"))) = {
    [0] = &_end_stack,
    [1] = reset_handler,
    [2] = nmi_handler,
    [3] = hardfault_handler,
    [11] = svcall_handler,
    [14] = pendsv_handler,
    [15] = systick_handler,
};

void wwdg_isr_handler(void) __attribute__((weak, alias("default_handler")));
void pvd_isr_handler(void) __attribute__((weak, alias("default_handler")));
void rtc_isr_handler(void) __attribute__((weak, alias("default_handler")));
void flash_isr_handler(void) __attribute__((weak, alias("default_handler")));
void rcc_isr_handler(void) __attribute__((weak, alias("default_handler")));
void exti0_1_isr_handler(void) __attribute__((weak, alias("default_handler")));
void exti2_3_isr_handler(void) __attribute__((weak, alias("default_handler")));
void exti4_15_isr_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_channel1_isr_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_channel2_3_isr_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_channel4_5_6_7_isr_handler(void) __attribute__((weak, alias("default_handler")));
void adc1_comp_isr_handler(void) __attribute__((weak, alias("default_handler")));
void lptim1_isr_handler(void) __attribute__((weak, alias("default_handler")));
void tim2_isr_handler(void) __attribute__((weak, alias("default_handler")));
void tim6_isr_handler(void) __attribute__((weak, alias("default_handler")));
void tim21_isr_handler(void) __attribute__((weak, alias("default_handler")));
void tim22_isr_handler(void) __attribute__((weak, alias("default_handler")));
void i2c1_isr_handler(void) __attribute__((weak, alias("default_handler")));
void i2c2_isr_handler(void) __attribute__((weak, alias("default_handler")));
void spi1_isr_handler(void) __attribute__((weak, alias("default_handler")));
void spi2_isr_handler(void) __attribute__((weak, alias("default_handler")));
void usart1_isr_handler(void) __attribute__((weak, alias("default_handler")));
void usart2_isr_handler(void) __attribute__((weak, alias("default_handler")));
void lpuart1_isr_handler(void) __attribute__((weak, alias("default_handler")));

void * const isr_vector_table[] __attribute__ ((section(".vendor_vectors"))) = {
    [WWDG_IRQn]                       = wwdg_isr_handler,
    [PVD_IRQn]                        = pvd_isr_handler,
    [RTC_IRQn]                        = rtc_isr_handler,
    [FLASH_IRQn]                      = flash_isr_handler,
    [RCC_IRQn]                        = rcc_isr_handler,
    [EXTI0_1_IRQn]                    = exti0_1_isr_handler,
    [EXTI2_3_IRQn]                    = exti2_3_isr_handler,
    [EXTI4_15_IRQn]                   = exti4_15_isr_handler,
    [DMA1_Channel1_IRQn]              = dma1_channel1_isr_handler,
    [DMA1_Channel2_3_IRQn]            = dma1_channel2_3_isr_handler,
    [DMA1_Channel4_5_6_7_IRQn]        = dma1_channel4_5_6_7_isr_handler,
    [ADC1_COMP_IRQn]                  = adc1_comp_isr_handler,
    [LPTIM1_IRQn]                     = lptim1_isr_handler,
    [TIM2_IRQn]                       = tim2_isr_handler,
    [TIM6_IRQn]                       = tim6_isr_handler,
    [TIM21_IRQn]                      = tim21_isr_handler,
    [TIM22_IRQn]                      = tim22_isr_handler,
    [I2C1_IRQn]                       = i2c1_isr_handler,
    [I2C2_IRQn]                       = i2c2_isr_handler,
    [SPI1_IRQn]                       = spi1_isr_handler,
    [SPI2_IRQn]                       = spi2_isr_handler,
    [USART1_IRQn]                     = usart1_isr_handler,
    [USART2_IRQn]                     = usart2_isr_handler,
    [LPUART1_IRQn]                    = lpuart1_isr_handler,
};
