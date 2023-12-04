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
#include "nrf_delay.h"
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

#define ID_NUMBER 4235
#define digit1 ID_NUMBER%10
#define digit2 ID_NUMBER/10%10
#define digit3 ID_NUMBER/100%10
#define digit4 ID_NUMBER/1000%10

#define LEDS_NUMBER 4

#define BLINK_DELAY_MS 300

#define BUTTON NRF_GPIO_PIN_MAP(1,6)


void operator delete(void*, unsigned int)
{}


nrf::atomic_32 blink_enable;


struct LED
{
    size_t led_id;
    size_t blink_times;
} leds_list[LEDS_NUMBER] = { { 0 , digit4 },{ 1, digit3 },{ 2, digit2 },{ 3 , digit1 } };

LED* leds = leds_list;


void my_handler(nrf::button_events evt)
{
    blink_enable = true;

    if (evt == nrf::on_click_up)
        blink_enable = false;
}


void blink_timer_handler(error::error_status e)
{
    static size_t cnt;

    if (cnt++ < leds->blink_times * 2)
    {
        bsp_board_led_invert(leds->led_id);
    }
    else
    {
        if (leds->led_id != 3)
            ++leds;
        else
            leds = leds_list;

        cnt = 0;
    }
}


int main(void)
{
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);

    nrf::debounced_button<BUTTON> a(my_handler);

    nrf::singleshot_apptimer blink_timer;

    while (true)
    {
        if (blink_enable)
            blink_timer.async_wait(BLINK_DELAY_MS, blink_timer_handler);
    }


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
