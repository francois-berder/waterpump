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

#include "app/board.h"
#include "app/pumps.h"
#include "app/sim800l.h"
#include "mcu/gpio.h"
#include "mcu/mcu.h"
#include "mcu/stm32l051xx.h"
#include "mcu/uart.h"

#define SIMCARD_PIN     (1234)

static struct sim800l_params_t gsm_params = {
    .dev = USART2,
};

/* push button req water callback */
void ext5_callback(void)
{
    pumps_start();
}

static void init_clocks(void)
{
    /* Enable HSI clock */
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    /* Configure AHB, APB1, APB2 clocks */
    RCC->CFGR = (RCC_CFGR_SW_HSI | RCC_CFGR_HPRE_DIV1 |
                 RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_PPRE2_DIV1);
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);

    /* Enable LSI clock (for RTC) */
    RCC->CSR |= RCC_CSR_LSION;
    while (!(RCC->CSR & RCC_CSR_LSIRDY));

    /* Clock RTC using LSI */
    RCC->CSR &= ~RCC_CSR_RTCSEL;
    RCC->CSR |= 0x2 << RCC_CSR_RTCSEL_Pos;

    /* Disable write protection for RTC module */
    PWR->CR |= PWR_CR_DBP;
}

int main(void)
{
    enum sim800l_network_registration_status_t status;
    enum sim800l_sim_status_t sim_status;
    int ret;

    init_clocks();

    /* Initialize GPIOs */
    gpio_init_out(LED_PIN, 1);
    gpio_init_out(ENABLE_GSM_PIN, 0);
    gpio_init_out(ENABLE_PUMP1_PIN, 0);
    gpio_init_out(ENABLE_PUMP2_PIN, 0);
    gpio_init_int(REQ_WATER_PIN, PULL_NONE, GPIO_FALLING);
    gpio_init_out(GSM_TX_PIN, 1);
    gpio_init_in(GSM_RX_PIN, PULL_UP);
    gpio_init_af(GSM_TX_PIN, 4);
    gpio_init_af(GSM_RX_PIN, 4);

    uart_power_up(USART2);
    uart_set_rx_callback(USART2, sim800l_receive_cb);
    uart_init(USART2, 115200);
    uart_enable(USART2);

    /* Configure NVIC and enable interrupts */
    NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_EnableIRQ(USART2_IRQn);
    __enable_irq();

    /* Initialize SIM800 module */
    mcu_delay(1000);
    gpio_write(ENABLE_GSM_PIN, 1);
    mcu_delay(5000);

    if (sim800l_get_sim_status(&gsm_params, &sim_status)) {
        gpio_write(ENABLE_GSM_PIN, 0);
        goto main_loop;
    }

    if (sim_status == SIM_PIN_LOCK) {
        if (sim800l_unlock_sim(&gsm_params, SIMCARD_PIN)) {
            gpio_write(ENABLE_GSM_PIN, 0);
            goto main_loop;
        }
    } else if (sim_status != SIM_READY) {
        gpio_write(ENABLE_GSM_PIN, 0);
        goto main_loop;
    }

    /* Give it a bit of time to register to the network */
    mcu_delay(10000);

    /* Check network registration status */
    ret = sim800l_check_network_registration(&gsm_params, &status);
    if (ret || status == SIM800_NOT_REGISTERED)
        gpio_write(ENABLE_GSM_PIN, 0);

main_loop:
    while (1)
        __asm__ volatile ("wfi" ::: "memory");

    __builtin_unreachable();
}
