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


#include "app/sim800l.h"
#include "mcu/mcu.h"
#include "mcu/timer.h"
#include "mcu/uart.h"
#include <stdio.h>
#include <string.h>

#define LINE_COUNT          (4)
#define MAX_LINE_LENGTH     (256)

enum cmd_status_t {
    CMD_STATUS_OK,
    CMD_STATUS_ERROR,
    CMD_STATUS_LINE_OVERRUN,
    CMD_STATUS_LINE_TOO_LONG,
    CMD_STATUS_TIMEOUT,
    CMD_STATUS_ONGOING,
};
static volatile enum cmd_status_t status;

enum cmd_t {
    CMD_SIM_STATUS,
    CMD_SIM_UNLOCK,
    CMD_SIM_NETWORK_REG_STATUS,
    CMD_SET_SMS_FORMAT,
    CMD_SET_SMS_STORAGE,
    CMD_DELETE_ALL_SMS,
    CMD_DELETE_SMS,
};
static enum cmd_t current_cmd;

struct __attribute__((packed)) line_t {
    char line[MAX_LINE_LENGTH];
    uint8_t ready_for_processing;
    uint8_t length;
};

static struct line_t lines[LINE_COUNT];
static uint8_t line_write_index;

static uint8_t line_read_index;
static volatile uint8_t line_available_count;

static volatile uint32_t time_remaining;    /**< used for timeout, in millisecond */

union cmd_result_t {
    struct {
        enum sim800l_network_registration_status_t status;
    } net_reg_status;
    struct {
        enum sim800l_sim_status_t status;
    } sim_status;
};
static union cmd_result_t result;

void sim800l_receive_cb(char c)
{
    int length;

    if (status != CMD_STATUS_ONGOING)
        return;

    length = lines[line_write_index].length;
    lines[line_write_index].line[length] = c;
    length++;
    lines[line_write_index].length = length;
    if (length == MAX_LINE_LENGTH) {
        /* It means there is no space for NULL character */
        status = CMD_STATUS_LINE_TOO_LONG;
    } else if (c == '\n') {
        lines[line_write_index].line[length] = '\0';
        lines[line_write_index].ready_for_processing = 1;

        line_available_count++;

        /* Start using next line */
        line_write_index++;
        if (line_write_index == LINE_COUNT)
            line_write_index = 0;

        /* Was it processed ? */
        if (lines[line_write_index].ready_for_processing)
            status = CMD_STATUS_LINE_OVERRUN;
    }
}

static void process_line(void)
{
    char *line = lines[line_read_index].line;
    uint8_t length = lines[line_read_index].length;

    if (!strcmp("OK\r\n", line)) {
        status = CMD_STATUS_OK;
    } else if (!strcmp("ERROR\r\n", line)) {
        status = CMD_STATUS_ERROR;
    } else if (current_cmd == CMD_SIM_NETWORK_REG_STATUS) {
        if (!strncmp("+CREG: ", line, 7) && length >= 10) {
            if (line[9] == '1')
                result.net_reg_status.status = SIM800_REGISTERED;
            else if (line[9] == '5')
                result.net_reg_status.status = SIM800_ROAMING;
            else
                result.net_reg_status.status = SIM800_NOT_REGISTERED;
        }
    } else if (current_cmd == CMD_SIM_STATUS) {
        if (!strcmp("+CPIN: READY\r\n", line))
            result.sim_status.status = SIM_READY;
        else if (!strcmp("+CPIN: SIM PIN\r\n", line))
            result.sim_status.status = SIM_PIN_LOCK;
        else if (!strcmp("+CPIN: SIM PUK\r\n", line))
            result.sim_status.status = SIM_PUK_LOCK;
        else if (!strncmp("+CPIN: ", line, 7))
            result.sim_status.status = SIM_ERROR;
    }

    /* Make line available for receiving data from SIM800 */
    lines[line_read_index].length = 0;
    lines[line_read_index].ready_for_processing = 0;

    line_read_index++;
    if (line_read_index == LINE_COUNT)
        line_read_index = 0;
}

static void cmd_timeout(void *arg)
{
    (void)arg;

    if (time_remaining >= 5)
        time_remaining -= 5;
    else
        status = CMD_STATUS_TIMEOUT;
}

static void wait_for_cmd_completion(void)
{
    status = CMD_STATUS_ONGOING;

    timer_power_up(TIM21);
    timer_init(TIM21, 200, 999);
    timer_init_channel(TIM21, TIMER_CHANNEL_1, 999, cmd_timeout, NULL);
    timer_start(TIM21);

    while (status == CMD_STATUS_ONGOING) {
        int line_to_process = 0;

        __disable_irq();
        if (line_available_count) {
            line_available_count--;
            line_to_process = 1;
        }
        __enable_irq();

        if (line_to_process)
            process_line();

        /* Sleep while we wait for command completion */
        __asm__ volatile ("wfi" ::: "memory");
    }

    timer_power_down(TIM21);
}

int sim800l_check_sim_card_present(struct sim800l_params_t *params)
{
    uart_send(params->dev, "AT+CCID\r\n", 9);
    current_cmd = CMD_SIM_STATUS;
    time_remaining = 500;
    wait_for_cmd_completion();

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_get_sim_status(struct sim800l_params_t *params, enum sim800l_sim_status_t *sim_status)
{
    uart_send(params->dev, "AT+CPIN?\r\n", 10);
    current_cmd = CMD_SIM_STATUS;
    time_remaining = 500;
    wait_for_cmd_completion();

    if (status == CMD_STATUS_OK && sim_status)
        *sim_status = result.sim_status.status;

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_unlock_sim(struct sim800l_params_t *params, uint32_t pin)
{
    char cmd[15] = "AT+CPIN=XXXX\r\n";

    /* @todo Check that we actually need to provide PIN code */

    cmd[8] = '0' + ((pin / 1000) % 10);
    cmd[9] = '0' + ((pin / 100) % 10);
    cmd[10] = '0' + ((pin / 10) % 10);
    cmd[11] = '0' + (pin % 10);

    uart_send(params->dev, cmd, 15);
    current_cmd = CMD_SIM_UNLOCK;
    time_remaining = 500;
    wait_for_cmd_completion();

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_check_network_registration(struct sim800l_params_t *params, enum sim800l_network_registration_status_t *net_reg_status)
{
    uart_send(params->dev, "AT+CREG?\r\n", 10);
    current_cmd = CMD_SIM_NETWORK_REG_STATUS;
    time_remaining = 500;
    wait_for_cmd_completion();

    if (status == CMD_STATUS_OK && net_reg_status)
        *net_reg_status = result.net_reg_status.status;

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_set_sms_format_to_text(struct sim800l_params_t *params)
{
    uart_send(params->dev, "AT+CMGF=1\r\n", 11);
    current_cmd = CMD_SET_SMS_FORMAT;
    time_remaining = 500;
    wait_for_cmd_completion();

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_use_simcard_for_sms_storage(struct sim800l_params_t *params)
{
    uart_send(params->dev, "AT+CPMS=\"SM\"\r\n", 14);
    current_cmd = CMD_SET_SMS_STORAGE;
    time_remaining = 500;
    wait_for_cmd_completion();

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_delete_all_sms(struct sim800l_params_t *params)
{
    uart_send(params->dev, "AT+CMGDA=\"DEL ALL\"\r\n", 20);
    current_cmd = CMD_DELETE_ALL_SMS;
    time_remaining = 500;
    wait_for_cmd_completion();

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_delete_sms(struct sim800l_params_t *params, uint8_t index)
{
    if (index < 10) {
        char *cmd = "AT+CMGD=X\r\n";
        cmd[8] = '0' + index;
        uart_send(params->dev, cmd, 11);
    } else {
        char *cmd = "AT+CMGD=XX\r\n";
        cmd[8] = '0' + ((index / 10) % 10);
        cmd[9] = '0' + (index % 10);
        uart_send(params->dev, cmd, 12);
    }
    current_cmd = CMD_DELETE_SMS;
    time_remaining = 500;
    wait_for_cmd_completion();

    return status == CMD_STATUS_OK ? 0 : -1;
}
