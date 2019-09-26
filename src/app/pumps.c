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
#include "mcu/gpio.h"
#include "mcu/timer.h"
#include "mcu/stm32l051xx.h"
#include <stdbool.h>
#include <stddef.h>

static bool running;
static int counter;

static void pumps_stop(void *arg)
{
    (void)arg;

    counter++;
    if (counter < 3)
        return;

    gpio_write(ENABLE_PUMP1_PIN, 0);
    gpio_write(ENABLE_PUMP2_PIN, 0);

    timer_power_down(TIM2);

    running = false;
}

void pumps_start(void)
{
    if (running)
        return;


    gpio_write(ENABLE_PUMP1_PIN, 1);
    gpio_write(ENABLE_PUMP2_PIN, 1);

    running = true;
    counter = 0;

    /*
     * Configure timer to call pumps_stop at 1Hz.
     * After three seconds, pumps are disabled and the
     * timer is stopped.
     */
    timer_power_up(TIM2);
    timer_init(TIM2, 1, 9999);
    timer_init_channel(TIM2, TIMER_CHANNEL_1, 9999, pumps_stop, NULL);
    timer_start(TIM2);
}
