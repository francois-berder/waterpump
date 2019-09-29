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
#include "mcu/mcu.h"
#include "mcu/stm32l051xx.h"

void mcu_delay(unsigned int milliseconds)
{
    unsigned int secs = milliseconds / 1000;
    milliseconds = milliseconds - secs * 1000;

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while (secs) {
        SysTick->LOAD = CORE_CLOCK - 1;
        SysTick->VAL = 0;
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
        secs--;
    }
    if (milliseconds) {
        SysTick->LOAD = (CORE_CLOCK / 1000) * milliseconds - 1;
        SysTick->VAL = 0;
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
