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


#include "board.h"
#include "mcu/timer.h"
#include "status.h"
#include <stddef.h>

static void blink_led(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

void status_off(void)
{
    timer_power_down(TIM22);
    gpio_write(LED_PIN, 0);
}

void status_blink(void)
{
    timer_power_up(TIM22);
    timer_init(TIM22, 10, 9999);
    timer_init_channel(TIM22, TIMER_CHANNEL_1, 9999, blink_led, NULL);
    timer_start(TIM22);
}
