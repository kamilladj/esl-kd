#pragma once

#include "utils/static_function.hpp"
#include "error/error_status.hpp"
#include "singleshot_apptimer.hpp"
#include "async_button.hpp"

#define DEBOUNCE_TIMER_TIME_MS 10

namespace nrf
{
    template<nrfx_gpiote_pin_t PIN>
    class smart_button
    {
    public:

        smart_button(utils::static_function<void(error::error_status)> handler)
            : m_debounce_timer{}
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
                    m_debounce_timer.async_wait(DEBOUNCE_TIMER_TIME_MS, [this](error::error_status e) { timer_handler(e); });
            }
        }

        void timer_handler(error::error_status e)
        {
            m_handler(e);
        }

    private:

        singleshot_apptimer                               m_debounce_timer;
        async_button<PIN>                                 m_async_button;
        utils::static_function<void(error::error_status)> m_handler;
    };
}