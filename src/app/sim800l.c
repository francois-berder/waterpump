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
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifndef SIM800_APN_NAME
#error "SIM800_APN_NAME is not set"
#endif

#define LINE_COUNT          (4)
#define MAX_LINE_LENGTH     (256)

/* Position of fields in sms header time variable */
#define YEAR_DEC_POS    (44)
#define YEAR_DIGIT_POS  (40)
#define MONTH_DEC_POS   (36)
#define MONTH_DIGIT_POS (32)
#define DAY_DEC_POS     (28)
#define DAY_DIGIT_POS   (24)
#define HOUR_DEC_POS    (20)
#define HOUR_DIGIT_POS  (16)
#define MIN_DEC_POS     (12)
#define MIN_DIGIT_POS   (8)
#define SEC_DEC_POS     (4)
#define SEC_DIGIT_POS   (0)

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
    CMD_CONNECT_TO_NETWORK,
    CMD_SET_SMS_FORMAT,
    CMD_SET_SMS_STORAGE,
    CMD_DELETE_ALL_SMS,
    CMD_DELETE_ALL_RECEIVED_SMS,
    CMD_DELETE_SMS,
    CMD_READ_UNREAD_SMS,
    CMD_SEND_SMS,
    CMD_ENABLE_TIME,
    CMD_GET_TIME,
    CMD_SYNC_TIME,
    CMD_DELETE_ALARM,
    CMD_SET_ALARM,
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
    struct {
        uint64_t time;
    } network_time;
};
static union cmd_result_t result;

static sim800l_receive_sms_callback_t sms_receive_cb;
static struct sim800l_sms_t sms;
static bool parsing_sms;        /**< Keep track whether we received some parts of a SMS */

static sim800l_alarm_cb_t alarm_callbacks[SIM800L_ALARM_COUNT];

static void parse_sms_header(char *buffer)
{
    char *ptr;
    int sender_length;

    ptr = &buffer[7];

    /* Parse index field */
    sms.header.index = 0;
    while (*ptr != ',') {
        uint8_t digit = *ptr - '0';
        sms.header.index = sms.header.index * 10 + digit;

        ptr++;
    }

    /* Skip status field */
    while (*ptr++ != ',');
    while (*ptr++ != ',');

    /* Parse address field */
    ptr++;  /* Skip " */
    sender_length = 0;
    while (*ptr != '\"')
        sms.header.sender[sender_length++] = *ptr++;
    sms.header.sender[sender_length] = '\0';

    /* Skip address text field */
    while (*ptr++ != ',');
    while (*ptr++ != ',');

    /* Skip " */
    ptr++;

    /* Parse time */
    sms.header.time = 0;
    sms.header.time |= (uint64_t)(*ptr++ - '0') << YEAR_DEC_POS;
    sms.header.time |= (uint64_t)(*ptr++ - '0') << YEAR_DIGIT_POS;
    ptr++;
    sms.header.time |= (uint64_t)(*ptr++ - '0') << MONTH_DEC_POS;
    sms.header.time |= (uint64_t)(*ptr++ - '0') << MONTH_DIGIT_POS;
    ptr++;
    sms.header.time |= (uint64_t)(*ptr++ - '0') << DAY_DEC_POS;
    sms.header.time |= (uint64_t)(*ptr++ - '0') << DAY_DIGIT_POS;
    ptr++;
    sms.header.time |= (uint64_t)(*ptr++ - '0') << HOUR_DEC_POS;
    sms.header.time |= (uint64_t)(*ptr++ - '0') << HOUR_DIGIT_POS;
    ptr++;
    sms.header.time |= (uint64_t)(*ptr++ - '0') << MIN_DEC_POS;
    sms.header.time |= (uint64_t)(*ptr++ - '0') << MIN_DIGIT_POS;
    ptr++;
    sms.header.time |= (uint64_t)(*ptr++ - '0') << SEC_DEC_POS;
    sms.header.time |= (uint64_t)(*ptr++ - '0') << SEC_DIGIT_POS;
}

void sim800l_receive_cb(char c)
{
    int length;

    length = lines[line_write_index].length;
    lines[line_write_index].line[length] = c;
    length++;
    lines[line_write_index].length = length;
    if (length == MAX_LINE_LENGTH) {
        /* It means there is no space for NULL character */
        status = CMD_STATUS_LINE_TOO_LONG;
    } else if (c == '\n') {
        lines[line_write_index].line[length] = '\0';

        if (status == CMD_STATUS_ONGOING) {
            lines[line_write_index].ready_for_processing = 1;

            line_available_count++;

            /* Start using next line */
            line_write_index++;
            if (line_write_index == LINE_COUNT)
                line_write_index = 0;

            /* Was it processed ? */
            if (lines[line_write_index].ready_for_processing)
                status = CMD_STATUS_LINE_OVERRUN;
        } else {
            lines[line_write_index].length = 0;
            if (!strncmp(lines[line_write_index].line, "+CALV: ", 7)) {
                uint8_t alarm_index = lines[line_write_index].line[7] - '0';
                sim800l_alarm_cb_t cb = alarm_callbacks[alarm_index];
                if (cb)
                    cb(alarm_index);
            }
        }
    }
}

static void process_line(void)
{
    char *line = lines[line_read_index].line;
    uint8_t length = lines[line_read_index].length;

    if (!strcmp("OK\r\n", line)) {
        status = CMD_STATUS_OK;
        if (current_cmd == CMD_READ_UNREAD_SMS && parsing_sms) {
            if (sms_receive_cb)
                sms_receive_cb(&sms);
        }
    } else if (!strcmp("ERROR\r\n", line)) {
        status = CMD_STATUS_ERROR;
    } else if (current_cmd == CMD_SIM_NETWORK_REG_STATUS) {
        if (!strncmp("+CREG: ", line, 7) && length >= 10) {
            if (line[9] == '1')
                result.net_reg_status.status = SIM800_REGISTERED;
            else if (line[9] == '2')
                result.net_reg_status.status = SIM800_REGISTERING;
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
    } else if (current_cmd == CMD_READ_UNREAD_SMS) {
        if (!strncmp("+CMGL: ", line, 7)) {
            if (parsing_sms) {
                if (sms_receive_cb)
                    sms_receive_cb(&sms);
            }

            parse_sms_header(line);
            sms.text_length = 0;
            sms.text[0] = '\0';
            parsing_sms = true;
        } else if (parsing_sms) {
            sms.text_length += strlen(line);
            strcat(sms.text, line);
        }
    } else if (current_cmd == CMD_GET_TIME) {
        if (!strncmp("+CCLK: ", line, 7)) {
            char *ptr = &line[8];

            result.network_time.time = 0;
            result.network_time.time |= (uint64_t)(*ptr++ - '0') << YEAR_DEC_POS;
            result.network_time.time |= (uint64_t)(*ptr++ - '0') << YEAR_DIGIT_POS;
            ptr++;
            result.network_time.time |= (uint64_t)(*ptr++ - '0') << MONTH_DEC_POS;
            result.network_time.time |= (uint64_t)(*ptr++ - '0') << MONTH_DIGIT_POS;
            ptr++;
            result.network_time.time |= (uint64_t)(*ptr++ - '0') << DAY_DEC_POS;
            result.network_time.time |= (uint64_t)(*ptr++ - '0') << DAY_DIGIT_POS;
            ptr++;
            result.network_time.time |= (uint64_t)(*ptr++ - '0') << HOUR_DEC_POS;
            result.network_time.time |= (uint64_t)(*ptr++ - '0') << HOUR_DIGIT_POS;
            ptr++;
            result.network_time.time |= (uint64_t)(*ptr++ - '0') << MIN_DEC_POS;
            result.network_time.time |= (uint64_t)(*ptr++ - '0') << MIN_DIGIT_POS;
            ptr++;
            result.network_time.time |= (uint64_t)(*ptr++ - '0') << SEC_DEC_POS;
            result.network_time.time |= (uint64_t)(*ptr++ - '0') << SEC_DIGIT_POS;
        }
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

int sim800l_connect_to_network(struct sim800l_params_t *params)
{
    uart_send(params->dev, "AT+CREG=1\r\n", 11);
    current_cmd = CMD_CONNECT_TO_NETWORK;
    time_remaining = 500;
    wait_for_cmd_completion();

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

int sim800l_delete_all_received_sms(struct sim800l_params_t *params)
{
    uart_send(params->dev, "AT+CMGDA=\"DEL INBOX\"\r\n", 22);
    current_cmd = CMD_DELETE_ALL_RECEIVED_SMS;
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

int sim800l_read_all_unread_sms(struct sim800l_params_t *params, sim800l_receive_sms_callback_t cb)
{
    parsing_sms = false;
    sms_receive_cb = cb;
    uart_send(params->dev, "AT+CMGL=\"REC UNREAD\"\r\n", 22);
    current_cmd = CMD_READ_UNREAD_SMS;
    time_remaining = 5000;
    wait_for_cmd_completion();

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_send_sms(struct sim800l_params_t *params, const char *dest, const char *text)
{
    char cmd[128] = "AT+CMGS=\"";

    strcat(cmd, dest);
    strcat(cmd, "\"\r\n");
    uart_send(params->dev, cmd, strlen(cmd));

    /* A small delay must be present between cmd and text */
    mcu_delay(10);

    uart_send(params->dev, text, strlen(text));
    uart_send(params->dev, "\x1a", 1);

    current_cmd = CMD_READ_UNREAD_SMS;
    time_remaining = 5000;
    wait_for_cmd_completion();

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_enable_time_update_from_network(struct sim800l_params_t *params)
{
    uart_send(params->dev, "AT+CLTS=1\r\n", 11);
    current_cmd = CMD_ENABLE_TIME;
    time_remaining = 500;
    wait_for_cmd_completion();

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_get_time(struct sim800l_params_t *params, uint64_t *time)
{
    uart_send(params->dev, "AT+CCLK?\r\n", 10);
    current_cmd = CMD_GET_TIME;
    time_remaining = 500;
    wait_for_cmd_completion();
    if (status == CMD_STATUS_OK)
        *time = result.network_time.time;

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_sync_time(struct sim800l_params_t *params)
{
    int sync_time_done = 0;

    /* Configure and activate GPRS connection */
    uart_send(params->dev, "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n", 31);
    current_cmd = CMD_SYNC_TIME;
    time_remaining = 1000;
    wait_for_cmd_completion();
    if (status != CMD_STATUS_OK)
        return -1;

    uart_send(params->dev, "AT+SAPBR=3,1,\"APN\",\"" SIM800_APN_NAME "\"\r\n", 28);
    current_cmd = CMD_SYNC_TIME;
    time_remaining = 1000;
    wait_for_cmd_completion();
    if (status != CMD_STATUS_OK)
        return -1;

    uart_send(params->dev, "AT+SAPBR=1,1\r\n", 14);
    current_cmd = CMD_SYNC_TIME;
    time_remaining = 1000;
    wait_for_cmd_completion();
    if (status != CMD_STATUS_OK)
        return -1;

    /* Retrieve current network time and save it  */
    uart_send(params->dev, "AT+CIPGSMLOC=2,1\r\n", 18);
    current_cmd = CMD_SYNC_TIME;
    time_remaining = 15000;
    wait_for_cmd_completion();
    if (status == CMD_STATUS_OK)
        sync_time_done = 1;

    /* Deactivate GPRS connection */
    uart_send(params->dev, "AT+SAPBR=0,1\r\n", 14);
    current_cmd = CMD_SYNC_TIME;
    time_remaining = 1000;
    wait_for_cmd_completion();

    return (status == CMD_STATUS_OK && sync_time_done) ? 0 : -1;
}

int sim800l_delete_alarm(struct sim800l_params_t *params, uint8_t alarm_index)
{
    char cmd[16];

    if (alarm_index == 0 || alarm_index > SIM800L_ALARM_COUNT)
        return -1;

    strcpy(cmd, "AT+CALD=X\r\n");
    cmd[8] = '0' + alarm_index;

    uart_send(params->dev, cmd, 11);
    current_cmd = CMD_DELETE_ALARM;
    time_remaining = 500;
    wait_for_cmd_completion();

    return status == CMD_STATUS_OK ? 0 : -1;
}

int sim800l_set_alarm(struct sim800l_params_t *params, uint8_t alarm_index,
                      uint8_t hour, uint8_t min, uint8_t sec,
                      sim800l_alarm_cb_t cb)
{
    char cmd[32];

    if (alarm_index == 0 || alarm_index > SIM800L_ALARM_COUNT)
        return -1;

    strcpy(cmd, "AT+CALA=\"hh:mm:ss\",X,0\r\n");
    cmd[9] = '0' + ((hour >> 4) & 0xF);
    cmd[10] = '0' + (hour & 0xF);
    cmd[12] = '0' + ((min >> 4) & 0xF);
    cmd[13] = '0' + (min & 0xF);
    cmd[15] = '0' + ((sec >> 4) & 0xF);
    cmd[16] = '0' + (sec & 0xF);

    cmd[19] = '0' + alarm_index;
    uart_send(params->dev, cmd, strlen(cmd));
    current_cmd = CMD_SET_ALARM;
    time_remaining = 500;
    wait_for_cmd_completion();

    if (status != CMD_STATUS_OK)
        return - 1;

    alarm_callbacks[alarm_index] = cb;
    return 0;
}
