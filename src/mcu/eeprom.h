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

#ifndef PERIPH_EEPROM_H
#define PERIPH_EEPROM_H

#include <stdint.h>

/**
 * @brief Read data from EEPROM
 *
 * @param[in] address
 * @param[out] data
 * @param[in] length in bytes
 */
void eeprom_read(uint32_t address, void *data, uint32_t length);

/**
 * @brief Write data to EEPROM
 *
 * @param[in] address
 * @param[in] data
 * @param[in] length in bytes
 */
void eeprom_write(uint32_t address, void *data, uint32_t length);

#endif
