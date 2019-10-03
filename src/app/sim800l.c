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
#include "mcu/uart.h"
#include <stdio.h>
#include <string.h>

enum cmd_status_t {
    CMD_STATUS_OK,
    CMD_STATUS_ERROR,
    CMD_STATUS_TIMEOUT,
    CMD_STATUS_ONGOING,
};
static enum cmd_status_t status;

enum cmd_t {
    CMD_SIM_STATUS,
    CMD_SIM_UNLOCK,
    CMD_SIM_NETWORK_REG_STATUS,
};
static enum cmd_t current_cmd;

static char line[128];
static unsigned int length;

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
    if (status != CMD_STATUS_ONGOING)
        return;

    line[length++] = c;
    length &= 0x7F;

    if (c != '\n')
        return;

    line[length] = '\0';

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

    length = 0;
}

static void wait_for_cmd_completion(int timeout)
{
    while (timeout) {
        if (status == CMD_STATUS_OK || status == CMD_STATUS_ERROR)
            break;

        mcu_delay(5);
        if (timeout >= 5)
            timeout -= 5;
        else
            timeout = 0;
    }
    if (!timeout)
        status = CMD_STATUS_TIMEOUT;
}

int sim800l_get_sim_status(struct sim800l_params_t *params, enum sim800l_sim_status_t *sim_status)
{
    uart_send(params->dev, "AT+CPIN?\r\n", 10);
    current_cmd = CMD_SIM_STATUS;
    status = CMD_STATUS_ONGOING;
    wait_for_cmd_completion(500);

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
    status = CMD_STATUS_ONGOING;
    wait_for_cmd_completion(500);

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_check_network_registration(struct sim800l_params_t *params, enum sim800l_network_registration_status_t *net_reg_status)
{
    uart_send(params->dev, "AT+CREG?\r\n", 10);
    current_cmd = CMD_SIM_NETWORK_REG_STATUS;
    status = CMD_STATUS_ONGOING;
    wait_for_cmd_completion(500);

    if (status == CMD_STATUS_OK && net_reg_status)
        *net_reg_status = result.net_reg_status.status;

    return status == CMD_STATUS_OK ? 0 : -1;
}
