#pragma once

#include "boards.h"
#include "nrf_gpio.h"

#include "nrf/atomic_32.hpp"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include "app_usbd.h"
#include "app_usbd_serial_num.h"

#include "pwm.hpp"

enum blink_states { color_change_on, color_change_off };

namespace nrf
{
    class blink_event_pwm_manager
    {
    public:

        blink_event_pwm_manager()
            : m_state{ color_change_off }
            , m_pwm_instance{ [this]() { pwm_handler(); } }
        {}

    public:

        void enable(button_events evt)
        {
            if (evt == on_click_double)
            {
                NRF_LOG_INFO("Double click");
                m_pwm_instance.double_click_handler();
            }
            else if (evt == on_click_down)
            {
                NRF_LOG_INFO("Click down");
                m_state = color_change_on;
            }
            else
            {
                NRF_LOG_INFO("Click up");
                m_state = color_change_off;
            }
        }

        void pwm_handler()
        {
            m_pwm_instance.update_led1();
            if (m_state == color_change_on)
                m_pwm_instance.update_hsv();

            m_pwm_instance.update_led2();
        }

    private:

        atomic_32 m_state;
        pwm<0>    m_pwm_instance;
    };
}