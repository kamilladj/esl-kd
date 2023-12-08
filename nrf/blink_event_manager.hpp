#pragma once

#include "boards.h"
#include <stdbool.h>
#include <stddef.h>

#define ID 4231
#define DIGIT1 ID/1000%10
#define DIGIT2 ID/100%10
#define DIGIT3 ID/10%10
#define DIGIT4 ID%10


namespace nrf
{
    class blink_event_manager
    {
    public:

        blink_event_manager()
            : m_cur_index{ 0 }
            , m_count{ 0 }
            , m_is_blink_enabled{ false }
            , m_all_leds{ 0, 0, 0, 0, 1, 1, 2, 2, 2, 3 }
            , m_all_leds_size{ DIGIT1 + DIGIT2 + DIGIT3 + DIGIT4 }
        {}

    public:

        void enable(bool cond)
        {
            m_is_blink_enabled = cond;
        }

        void handle_event()
        {
            if (m_is_blink_enabled)
            {
                if (m_count++ < 2)
                {
                    bsp_board_led_invert(m_all_leds[m_cur_index]);
                }
                else
                {
                    if (m_cur_index == m_all_leds_size - 1)
                        m_cur_index = 0;
                    else
                        ++m_cur_index;

                    m_count = 0;
                }
            }
        }

    private:

        size_t       m_cur_index;
        size_t       m_count;
        bool         m_is_blink_enabled;
        const size_t m_all_leds[DIGIT1 + DIGIT2 + DIGIT3 + DIGIT4];
        const size_t m_all_leds_size;
    };
}