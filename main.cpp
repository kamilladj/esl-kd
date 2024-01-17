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
#include "nrf/blink_event_pwm_manager.hpp"

#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include "app_usbd.h"
#include "app_usbd_serial_num.h"

#include "nrfx_pwm.h"

/**
 * @brief Function for application main entry.
 */


#define BUTTON NRF_GPIO_PIN_MAP(1,6)


void operator delete(void*, unsigned int)
{}


void logs_init()
{
    NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


void button_event_handler(nrf::button_events evt, nrf::blink_event_pwm_manager& blink_manager)
{
    blink_manager.enable(evt);
}


int main(void)
{
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);

    logs_init();

    NRF_LOG_INFO("Starting up the test project with USB logging");

    nrf::blink_event_pwm_manager blink_manager;

    nrf::smart_button<BUTTON> a([&blink_manager](nrf::button_events evt) { button_event_handler(evt, blink_manager); });

    while (true)
    {
        __SEV();
        __WFE();
        __WFE();

        LOG_BACKEND_USB_PROCESS();
        NRF_LOG_PROCESS();
    }
}

/**
 *@}
 **/
