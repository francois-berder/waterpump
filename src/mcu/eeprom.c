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

#include "mcu/eeprom.h"
#include "mcu/stm32l051xx.h"

void eeprom_read(uint32_t address, void *data, uint32_t length)
{
    uint8_t *dst;
    uint8_t *src;

    dst = data;
    src = (uint8_t *)address;
    while (length--)
        *dst++ = *src++;
}

void eeprom_write(uint32_t address, void *data, uint32_t length)
{
    uint8_t *dst;
    uint8_t *src;

    /* Unlock EEPROM */
    if (FLASH->PECR & FLASH_PECR_PELOCK) {
        FLASH->PEKEYR = 0x89ABCDEF;
        FLASH->PEKEYR = 0x02030405;
    }

    dst = (uint8_t *)address;
    src = data;
    while (length--)
        *dst++ = *src++;

    /* Lock EEPROM */
    while (FLASH->SR & FLASH_SR_BSY);
    FLASH->PECR |= FLASH_PECR_PELOCK;
}
