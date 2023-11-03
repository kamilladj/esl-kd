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

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"

#define ID_NUMBER 5236
#define digit1 ID_NUMBER%10
#define digit2 ID_NUMBER/10%10
#define digit3 ID_NUMBER/100%10
#define digit4 ID_NUMBER/1000%10

APP_TIMER_DEF(m_timer_123);

/**
 * @brief Function for application main entry.
 */

struct LED
{
    int led_id;
    int count;
} leds[4] = { { 0, digit4 },{ 1, digit3 },{ 2, digit2 },{ 3, digit1 } };

void* ptr = leds;
void operator delete(void*, unsigned int)
{}

void single_shot_timer_handler(void* p_context)
{
    LED* led = (LED*)p_context;
    static int cnt;

    if (cnt++ < led->count * 2)
    {
        bsp_board_led_invert(led->led_id);
        app_timer_start(m_timer_123, APP_TIMER_TICKS(300), p_context);
    }
    else
    {
        if (led->led_id != 3)
            p_context = ++led;
        else
            p_context = ptr;

        cnt = 0;
        app_timer_start(m_timer_123, APP_TIMER_TICKS(1000), p_context);
    }
}


int main(void)
{
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);

    nrf_drv_clock_init();
    nrf_drv_clock_lfclk_request(NULL);

    ret_code_t err_code = app_timer_init();

    if (err_code == NRF_SUCCESS)
    {
        err_code = app_timer_create(&m_timer_123, APP_TIMER_MODE_SINGLE_SHOT, single_shot_timer_handler);

        if (err_code == NRF_SUCCESS)
        {
            err_code = app_timer_start(m_timer_123, APP_TIMER_TICKS(1000), ptr); //start blinking
        }
    }
}

/**
 *@}
 **/
