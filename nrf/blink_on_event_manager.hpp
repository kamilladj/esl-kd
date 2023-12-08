#pragma once

#include "boards.h"
#include "nrf/atomic_32.hpp"

#define ID 4231
#define DIGIT1 ID/1000%10
#define DIGIT2 ID/100%10
#define DIGIT3 ID/10%10
#define DIGIT4 ID%10

const size_t g_all_leds_size = DIGIT1 + DIGIT2 + DIGIT3 + DIGIT4;
const size_t g_all_leds[] = { 0, 0, 0, 0, 1, 1, 2, 2, 2, 3 };

extern nrf::atomic_32 g_is_blink_enabled;

namespace nrf
{
    class blink_on_event_manager
    {
    public:

        blink_on_event_manager()
            : cur_index{ 0 }
            , count{ 0 }
        {}

    public:

        void handle_event()
        {
            if (g_is_blink_enabled)
            {
                if (count++ < 2)
                {
                    bsp_board_led_invert(g_all_leds[cur_index]);
                }
                else
                {
                    if (cur_index == g_all_leds_size - 1)
                        cur_index = 0;
                    else
                        ++cur_index;

                    count = 0;
                }
            }
        }

    private:

        size_t cur_index;
        size_t count;
    };
}