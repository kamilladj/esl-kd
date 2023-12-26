#pragma once

#include "utils/static_function.hpp"
#include "error/error_status.hpp"

#include "debounced_button.hpp"
#include "atomic_32.hpp"

#define DOUBLE_CLICK_TIMER_TIME_MS 500

namespace nrf
{
    template<nrfx_gpiote_pin_t PIN>
    class smart_button
    {
    public:

        smart_button(utils::static_function<void(button_events)> handler)
            : m_double_click_timer{}
            , m_click_num{}
            , m_debounced_button{ [this](button_events evt) { debounced_button_handler(evt); } }
            , m_handler{}
        {
            m_handler = handler;
        }

    private:

        void debounced_button_handler(button_events evt)
        {
            if (evt == on_click_down)
            {
                m_handler(on_click_down);
                m_click_num++;
                m_double_click_timer.async_wait(DOUBLE_CLICK_TIMER_TIME_MS, [this](error::error_status e) { double_click_timer_handler(e); });
            }
        }

        void double_click_timer_handler(error::error_status e)
        {
            if (!e)
            {
                if (m_click_num == 2)
                    m_handler(on_click_double);

                m_click_num = 0;
            }
        }

    private:

        singleshot_apptimer                         m_double_click_timer;
        atomic_32                                   m_click_num;
        debounced_button<PIN>                       m_debounced_button;
        utils::static_function<void(button_events)> m_handler;
    };
}