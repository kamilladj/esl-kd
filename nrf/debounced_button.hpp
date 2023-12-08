#pragma once

#include "utils/static_function.hpp"
#include "error/error_status.hpp"
#include "atomic_32.hpp"

#include "singleshot_apptimer.hpp"
#include "async_button.hpp"

#define DEBOUNCE_TIMER_TIME_MS 20

namespace nrf
{
    enum button_events { on_click_down, on_click_up, on_click_double };

    template<nrfx_gpiote_pin_t PIN>
    class debounced_button
    {
    public:

        debounced_button(utils::static_function<void(button_events)> handler)
            : m_debounce_timer{}
            , m_state{ true }
            , m_async_button{ [this]() { async_button_handler(); } }
            , m_handler{}
        {
            m_handler = handler;
        }

    private:

        void async_button_handler()
        {
            error::error_status err = m_debounce_timer.cancel();
            if (!err)
                m_debounce_timer.async_wait(DEBOUNCE_TIMER_TIME_MS, [this](error::error_status e) { debounce_button_handler(e); });
        }

        void debounce_button_handler(error::error_status e)
        {
            if (!e)
            {
                if (m_state)
                    m_handler(on_click_down);
                else
                    m_handler(on_click_up);

                m_state = !m_state;
            }
        }

    private:

        singleshot_apptimer                         m_debounce_timer;
        atomic_32                                   m_state;
        async_button<PIN>                           m_async_button;
        utils::static_function<void(button_events)> m_handler;
    };
}