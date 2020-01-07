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
#include "app/pumps.h"
#include "mcu/gpio.h"
#include "mcu/timer.h"
#include "mcu/stm32l051xx.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static bool running;
static int counter;
static uint8_t duration;
static enum pump_t pumps_to_enable;

static void pumps_stop(void *arg)
{
    (void)arg;

    counter++;

    switch (pumps_to_enable) {
    case PUMP_1:
        if (counter == duration) {
            gpio_write(ENABLE_PUMP1_PIN, 0);
        } else if (counter == duration + 1) {
            timer_power_down(TIM2);
            running = false;
            status_on();
        }
        break;
    case PUMP_2:
        if (counter == duration) {
            gpio_write(ENABLE_PUMP2_PIN, 0);
        } else if (counter == duration + 1) {
            timer_power_down(TIM2);
            running = false;
            status_on();
        }
        break;
    case PUMP_ALL:
        if (counter == duration) {
            gpio_write(ENABLE_PUMP1_PIN, 0);
        } else if (counter == duration + 1) {
            gpio_write(ENABLE_PUMP2_PIN, 1);
        } else if (counter == 2 * duration + 1) {
            gpio_write(ENABLE_PUMP2_PIN, 0);
        } else if (counter == 2 * (duration + 1)) {
            timer_power_down(TIM2);
            running = false;
            status_on();
        }
        break;
    }
}

void pumps_start(enum pump_t pumps, uint8_t _duration)
{
    if (running || !_duration)
        return;

    pumps_to_enable = pumps;

    if (pumps_to_enable == PUMP_1 || pumps_to_enable == PUMP_ALL)
        gpio_write(ENABLE_PUMP1_PIN, 1);
    else
        gpio_write(ENABLE_PUMP2_PIN, 1);

    running = true;
    counter = 0;
    duration = _duration;

    /* Configure timer to call pumps_stop at 1Hz. */
    timer_power_up(TIM2);
    timer_init(TIM2, 1, 9999);
    timer_init_channel(TIM2, TIMER_CHANNEL_1, 9999, pumps_stop, NULL);
    timer_start(TIM2);
    status_blink();
}
