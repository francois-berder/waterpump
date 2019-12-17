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
#include "app/schedule.h"
#include "app/sim800l.h"
#include "mcu/gpio.h"
#include "mcu/mcu.h"
#include "mcu/rtc.h"
#include "mcu/stm32l051xx.h"
#include "mcu/uart.h"
#include <string.h>

#define SIMCARD_PIN     (1234)

#define GSM_ERROR_COUNTER_THRESHOLD     (10)

static struct sim800l_params_t gsm_params = {
    .dev = USART2,
};
static uint32_t gsm_error_counter;

/* push button req water callback */
void ext5_callback(void)
{
    pumps_start(PUMP_ALL);
}

static void handle_sms(struct sim800l_sms_t *sms)
{
    if (sms->text_length < 9)
        return;

    if (sms->text_length >= 11 && !strncmp(sms->text, "WATER ALL\r\n", 11))
        pumps_start(PUMP_ALL);
    else if (!strncmp(sms->text, "WATER 1\r\n", 9))
        pumps_start(PUMP_1);
    else if (!strncmp(sms->text, "WATER 2\r\n", 9))
        pumps_start(PUMP_2);
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
}

static int gsm_init(void)
{
    enum sim800l_sim_status_t sim_status;

    if (sim800l_enable_time(&gsm_params))
        return -1;

    if (sim800l_check_sim_card_present(&gsm_params)
    ||  sim800l_get_sim_status(&gsm_params, &sim_status))
        return -1;

    if (sim_status == SIM_PIN_LOCK) {
        if (sim800l_unlock_sim(&gsm_params, SIMCARD_PIN))
            return -1;

        /* Give it a bit of time to unlock SIM card */
        mcu_delay(5000);

        if (sim800l_get_sim_status(&gsm_params, &sim_status))
            return -1;
        if (sim_status != SIM_READY)
            return -1;
    } else if (sim_status != SIM_READY) {
        return -1;
    }

    if (sim800l_set_sms_format_to_text(&gsm_params))
        return -1;

    /* Wait for SIM card to become "SMS READY" */
    mcu_delay(5000);

    if (sim800l_use_simcard_for_sms_storage(&gsm_params)
    ||  sim800l_delete_all_sms(&gsm_params))
        return -1;

    return 0;
}

static int gsm_update(void)
{
    enum sim800l_network_registration_status_t status;

    if (sim800l_check_network_registration(&gsm_params, &status))
        return -1;

    if (status == SIM800_NOT_REGISTERED) {
        if (sim800l_connect_to_network(&gsm_params))
            return -1;
    }

    if (sim800l_read_all_unread_sms(&gsm_params, handle_sms))
        return -1;

    return 0;
}

int main(void)
{
    int gsm_enabled = 0;

    init_clocks();

    /* Initialize GPIOs */
    gpio_init_out(LED_PIN, 1);
    gpio_init_out(ENABLE_GSM_PIN, 0);
    gpio_init_out(ENABLE_PUMP1_PIN, 0);
    gpio_init_out(ENABLE_PUMP2_PIN, 0);
    gpio_init_int(REQ_WATER_PIN, PULL_UP, GPIO_FALLING);
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
    NVIC_EnableIRQ(TIM21_IRQn);
    NVIC_EnableIRQ(TIM22_IRQn);
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_EnableIRQ(RTC_IRQn);
    __enable_irq();

    /* Initialize SIM800 module */
    gsm_enabled = 1;
    gpio_write(ENABLE_GSM_PIN, 1);
    mcu_delay(5000);

    if (gsm_init()) {
        gpio_write(ENABLE_GSM_PIN, 0);
        gsm_enabled = 0;
    }

    /* Initialize RTC */
    rtc_init();

    /* Use 2G network to set time */
    if (gsm_enabled) {
        uint64_t now;
        if (!sim800l_get_time(&gsm_params, &now)) {
            union rtc_time_t t;
            t.asUint64 = now;
            rtc_set_time(t);
        }
    }

    schedule_init();

    while (1) {
        __asm__ volatile ("wfi" ::: "memory");

        /*
         * If SIM800L waked us, let's wait it sent everything
         * before we attempt to read from UART
         */
        mcu_delay(20);

        if (!gsm_enabled)
            continue;

        if (gsm_update()) {
            gsm_error_counter++;
            if (gsm_error_counter > GSM_ERROR_COUNTER_THRESHOLD) {
                gpio_write(ENABLE_GSM_PIN, 0);
                gsm_enabled = 0;
            }
        } else {
            gsm_error_counter = 0;
        }
    }

    __builtin_unreachable();
}
