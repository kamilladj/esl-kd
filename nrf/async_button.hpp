#pragma once

#include "nrf_drv_gpiote.h"
#include "utils/static_function.hpp"
#include "error/error_status.hpp"
#include "singleton_gpiote.hpp"
#include "utils/static_vector.hpp"
#include "utils/lock_guard.hpp"
#include <stddef.h>


namespace nrf
{
    template<nrfx_gpiote_pin_t PIN>
    class async_button
    {
    public:

        async_button(utils::static_function<void(error::error_status)> handler)
        {
            nrf_mtx_init(&m_mtx);
            {
                lock_guard lk(m_mtx);
                m_handlers.push_back(handler);
            }
            gpiote_init();
        }

        ~async_button()
        {
            nrf_mtx_destroy(&m_mtx);
        }

    private:

        void gpiote_init()
        {
            singleton_gpiote::init();

            nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
            in_config.pull = NRF_GPIO_PIN_PULLUP;

            //Configuring 
            error::error_status err = nrfx_gpiote_in_init(PIN, &in_config, button_handler);

            if (!err)
                nrfx_gpiote_in_event_enable(PIN, true);
        }

    private:

        static void button_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
        {
            static size_t i = m_handlers.get_size();
            lock_guard lk(m_mtx);
            if (i-- > 0)
                (m_handlers[i])(error::error_status());
            else
                i = m_handlers.get_size();
        }

    private:

        static nrf_mtx_t m_mtx;
        static static_vector<utils::static_function<void(error::error_status)>, 5>  m_handlers;
    };

    template<nrfx_gpiote_pin_t PIN>
    static_vector<utils::static_function<void(error::error_status)>, 5> async_button<PIN>::m_handlers;

    template<nrfx_gpiote_pin_t PIN>
    nrf_mtx_t async_button<PIN>::m_mtx;
}