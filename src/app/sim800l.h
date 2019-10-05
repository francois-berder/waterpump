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

#ifndef SIM800L_H
#define SIM800L_H

#include "mcu/stm32l051xx.h"
#include <stdint.h>

struct sim800l_params_t {
    USART_TypeDef *dev;
};

enum sim800l_sim_status_t {
    SIM_READY,
    SIM_PIN_LOCK,
    SIM_PUK_LOCK,
    SIM_ERROR,
};

enum sim800l_network_registration_status_t {
    SIM800_NOT_REGISTERED,
    SIM800_REGISTERED,
    SIM800_ROAMING,
};

void sim800l_receive_cb(char c);

/**
 * @brief Check SIM card is present
 *
 * @param[in] params
 * @return 0 if SIM card is present, -1 otherwise
 */
int sim800l_check_sim_card_present(struct sim800l_params_t *params);

/**
 * @brief Get SIM status
 *
 * @param[in] params
 * @param[out] sim_status
 * @retval 0 if successful
 * @retval -1 if ca
 */
int sim800l_get_sim_status(struct sim800l_params_t *params, enum sim800l_sim_status_t *sim_status);

/**
 * @brief Unlock SIM card using PIN code
 *
 * @param[in] params
 * @param[in] pin 0..9999
 * @retval 0 if successful
 * @retval -1 if card could not be unlocked
 */
int sim800l_unlock_sim(struct sim800l_params_t *params, uint32_t pin);

/**
 * @brief Check registration status
 *
 * @param[in] params
 * @param[out] status
 * @retval 0 if SIM800L module is registered to the network
 * @retval -1 if an error occurred
 */
int sim800l_check_network_registration(struct sim800l_params_t *params, enum sim800l_network_registration_status_t *status);

/**
 * @brief Set SMS format to text
 *
 * @param[in] params
 * @retval 0 if SMS format is set to text
 * @retval -1 if an error occurred
 */
int sim800l_set_sms_format_to_text(struct sim800l_params_t *params);

/**
 * @brief Use SIM card to store SMS
 *
 * @param[in] params
 * @retval 0 if storage for SMS could not be set
 * @retval -1 if an error occurred
 */
int sim800l_use_simcard_for_sms_storage(struct sim800l_params_t *params);

#endif
