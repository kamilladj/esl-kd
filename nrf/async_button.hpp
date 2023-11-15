#pragma once

#include "nrf_drv_gpiote.h"
#include "utils/static_function.hpp"
#include "error/error_status.hpp"
#include "singleton_gpiote.hpp"


namespace nrf
{
    template<nrfx_gpiote_pin_t PIN>
    class async_button
    {
    public:

        async_button(utils::static_function<void(error::error_status)> handler)
        {
            m_handler = handler;
            gpiote_init();
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
            m_handler(error::error_status());
        }

    private:

        static utils::static_function<void(error::error_status)> m_handler;
    };

    template<nrfx_gpiote_pin_t PIN>
    utils::static_function<void(error::error_status)> async_button<PIN>::m_handler;
}