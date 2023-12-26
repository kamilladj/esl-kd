#pragma once

#include "boards.h"
#include "nrf_gpio.h"

#include "nrf/atomic_32.hpp"
#include "nrf/systick.hpp"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
            , m_timer{}
            , m_delay_us{ 0 }
            , m_is_led_on{ false }
            , m_period_us{ 1000 }
            , m_step_value{ 2 }
            , m_duty_cycle{ 0 }
            , m_double_click_num{ 0 }
        {}

    public:

        void enable(bool cond)
        {
            if (cond)
            {
                ++m_double_click_num;
                if (m_double_click_num == 1)
                {
                    m_blink_state = blink_on;
                }
                else
                {
                    m_blink_state = blink_off;
                    m_double_click_num = 0;
                }
            }
        }

        void handle_event()
        {
            if (m_blink_state == blink_on)
            {
                if (m_timer.test(m_delay_us))
                {
                    if (!m_is_led_on)
                        change_duty_cycle();

                    bsp_board_led_invert(m_all_leds[m_cur_index]);
                    m_is_led_on = !m_is_led_on;

                    if (m_duty_cycle == 0)
                    {
                        bsp_board_led_off(m_all_leds[m_cur_index]);
                        m_is_led_on = false;
                        ++m_cur_index;
                        m_cur_index %= m_all_leds_size;
                    }

                    m_delay_us = m_is_led_on ? get_time_on_us() : get_time_off_us();
                    m_timer.get();
                }
            }
        }

    public:

        uint32_t get_time_on_us()
        {
            return m_period_us * m_duty_cycle / 100;
        }

        uint32_t get_time_off_us()
        {
            return m_period_us - get_time_on_us();
        }

    public:

        void change_duty_cycle()
        {
            m_duty_cycle += m_step_value;

            if (m_duty_cycle == 0 || m_duty_cycle == 100)
                m_step_value *= -1;
        }

    private:

        size_t         m_cur_index;
        atomic_32      m_blink_state;
        const size_t   m_all_leds[DIGIT1 + DIGIT2 + DIGIT3 + DIGIT4];
        const size_t   m_all_leds_size;
        systick        m_timer;
        uint32_t       m_delay_us;
        atomic_32      m_is_led_on;
        const uint32_t m_period_us;
        int32_t        m_step_value;
        uint32_t       m_duty_cycle;
        atomic_32      m_double_click_num;
    };
}