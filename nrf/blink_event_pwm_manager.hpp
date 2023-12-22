#pragma once

#include "boards.h"
#include "nrf_gpio.h"

#include "nrf/atomic_32.hpp"
#include "nrf/systick.hpp"
#include "nrf/pwm.hpp"

#include <stdbool.h>
#include <stddef.h>

#include "nrf_delay.h"

#define ID 4231
#define DIGIT1 ID/1000%10
#define DIGIT2 ID/100%10
#define DIGIT3 ID/10%10
#define DIGIT4 ID%10

enum { blink_on, blink_off };

namespace nrf
{
    class blink_event_pwm_manager
    {
    public:

        blink_event_pwm_manager()
            : m_cur_index{ 0 }
            , m_blink_state{ blink_off }
            , m_all_leds{ 0, 0, 0, 0, 1, 1, 2, 2, 2, 3 }
            , m_all_leds_size{ DIGIT1 + DIGIT2 + DIGIT3 + DIGIT4 }
            , m_pwm{}
            , m_timer{}
        {}

    public:

        void enable(bool cond)
        {
            if (cond)
                m_blink_state = blink_on;
            else
                m_blink_state = blink_off;
        }

        void handle_event()
        {
            if (m_blink_state == blink_on)
            {
                m_pwm.change_duty_cycle();
                blink();

                if (m_pwm.get_duty_cycle() == 0)
                {
                    m_cur_index++;
                    m_cur_index %= m_all_leds_size;
                }
            }
        }

        void blink()
        {
            bsp_board_led_on(m_all_leds[m_cur_index]);
            m_timer.get();
            while (!m_timer.test(m_pwm.get_time_on_us()));
            bsp_board_led_off(m_all_leds[m_cur_index]);
            m_timer.get();
            while (!m_timer.test(m_pwm.get_time_off_us()));
        }

    private:

        size_t              m_cur_index;
        atomic_32           m_blink_state;
        const size_t        m_all_leds[DIGIT1 + DIGIT2 + DIGIT3 + DIGIT4];
        const size_t        m_all_leds_size;
        pwm                 m_pwm;
        systick             m_timer;
    };
}