/**
 * Copyright (c) 2014 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
 /** @file
  *
  * @defgroup blinky_example_main main.c
  * @{
  * @ingroup blinky_example
  * @brief Blinky Example Application main file.
  *
  * This file contains the source code for a sample application to blink LEDs.
  *
  */

#include "boards.h"
#include "nrf_gpio.h"

#include "error/error_status.hpp"

#include "nrf/async_button.hpp"
#include "nrf/debounced_button.hpp"
#include "nrf/smart_button.hpp"
#include "nrf/singleshot_apptimer.hpp"
#include "nrf/atomic_32.hpp"

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Function for application main entry.
 */


#define ID 4231
#define DIGIT1 ID/1000%10
#define DIGIT2 ID/100%10
#define DIGIT3 ID/10%10
#define DIGIT4 ID%10

#define BUTTON NRF_GPIO_PIN_MAP(1,6)


void operator delete(void*, unsigned int)
{}


const uint32_t g_delay_ms = 200;
const size_t   g_all_leds_size = DIGIT1 + DIGIT2 + DIGIT3 + DIGIT4;
nrf::atomic_32 g_is_blink_enabled;
size_t         g_all_leds[] = { 0, 0, 0, 0, 1, 1, 2, 2, 2, 3 };


void blink_timer_handler(error::error_status e, nrf::singleshot_apptimer& blink_timer)
{
    static size_t cur_index = 0;
    static size_t cnt = 0;

    if (g_is_blink_enabled)
    {
        if (cnt++ < 2)
        {
            bsp_board_led_invert(g_all_leds[cur_index]);
        }
        else
        {
            if (cur_index == g_all_leds_size - 1)
                cur_index = 0;
            else
                ++cur_index;

            cnt = 0;
        }
    }

    blink_timer.async_wait(g_delay_ms, [&blink_timer](error::error_status e) { blink_timer_handler(e, blink_timer); });
}


void button_event_handler(nrf::button_events evt, nrf::singleshot_apptimer& blink_timer)
{
    if (evt == nrf::on_click_up)
        g_is_blink_enabled = false;
    else
        g_is_blink_enabled = true;
}


int main(void)
{
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);

    nrf::singleshot_apptimer blink_timer;

    nrf::debounced_button<BUTTON> a([&blink_timer](nrf::button_events evt) { button_event_handler(evt, blink_timer); });

    while (!g_is_blink_enabled);
    blink_timer.async_wait(g_delay_ms, [&blink_timer](error::error_status e) { blink_timer_handler(e, blink_timer); });


    while (true)
    {
        __SEV();
        __WFE();
        __WFE();
    }
}

/**
 *@}
 **/
