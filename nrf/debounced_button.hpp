#pragma once

#include "utils/static_function.hpp"
#include "error/error_status.hpp"
#include "singleshot_apptimer.hpp"
#include "async_button.hpp"

#include <stdbool.h>

#define DEBOUNCE_TIMER_TIME_MS 20


namespace nrf
{
    //enum button_events {on_click_down, on_click_up, on_click_double};

    template<nrfx_gpiote_pin_t PIN>
    class debounced_button
    {
    public:

        debounced_button(utils::static_function<void(error::error_status)> handler)
            : m_debounce_timer{}
            , m_state{true}
            , m_async_button{ [this](error::error_status e) { async_button_handler(e); } }
            , m_handler{}
        {
                m_handler = handler;
        }

    private:

        void async_button_handler(error::error_status e)
        {
            if (!e)
            {
                error::error_status err = m_debounce_timer.cancel();
                if (!err)
                    m_debounce_timer.async_wait(DEBOUNCE_TIMER_TIME_MS, [this](error::error_status e) { debounce_timer_handler(e); });
            }
        }

        void debounce_timer_handler(error::error_status e)
        {
            if (m_state) //on down
                m_handler(error::error_status());

            m_state = !m_state;
        }

    private:

        singleshot_apptimer                               m_debounce_timer;
        volatile bool                                     m_state;
        async_button<PIN>                                 m_async_button;
        utils::static_function<void(error::error_status)> m_handler;
    };
}