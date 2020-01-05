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

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <stdint.h>

/**
 * @brief Apply configuration saved in EEPROM
 */
void schedule_init(void);

/**
 * @brief Configure schedule
 *
 * This enables the schedule.
 *
 * @param[in] index Must be 0 or 1
 * @param[in] hour in range 0..23
 * @param[in] min in range 0..59
 * @param[in] sec in range 0..59
 * @param[in] pumps
 */
void schedule_configure(int index, uint8_t hour, uint8_t min, uint8_t sec, enum pump_t pumps);

/**
 * @brief Enable schedule
 *
 * @param[in] index Must be 0 or 1
 */
void schedule_enable(int index);

/**
 * @brief Disable schedule
 *
 * @param[in] index Must be 0 or 1
 */
void schedule_disable(int index);

/**
 * @brief Print schedule status
 * 
 * @param[out] dst
 */
void schedule_to_string(char *dst);

#endif
