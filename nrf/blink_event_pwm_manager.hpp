#pragma once

#include "nrf/atomic_32.hpp"

#include "pwm.hpp"

enum blink_states { color_change_on, color_change_off };

namespace nrf
{
    class blink_event_pwm_manager
    {
    public:

        blink_event_pwm_manager()
            : m_state{ color_change_off }
            , m_pwm{ [this]() { pwm_handler(); } }
        {}

    public:

        void enable(button_events evt)
        {
            if (evt == on_click_double)
                m_pwm.double_click_handler();
            else if (evt == on_click_down)
                m_state = color_change_on;
            else
                m_state = color_change_off;
        }

        void pwm_handler()
        {
            m_pwm.update_led1();

            if (m_state == color_change_on)
                m_pwm.update_hsv();

            m_pwm.update_led2();
        }

    private:

        atomic_32 m_state;
        pwm<0>    m_pwm;
    };
}