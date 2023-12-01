#pragma once

#include "nrf_drv_gpiote.h"
#include "error/error_status.hpp"

namespace nrf
{
    template<nrfx_gpiote_pin_t PIN>
    class singleton_gpiote_config
    {
    public:

        static singleton_gpiote_config& init(nrfx_gpiote_evt_handler_t handler)
        {
            static singleton_gpiote_config s(handler);
            return s;
        }

        singleton_gpiote_config(const singleton_gpiote_config&) = delete;
        singleton_gpiote_config& operator=(const singleton_gpiote_config&) = delete;

    private:

        singleton_gpiote_config(nrfx_gpiote_evt_handler_t handler)
        {
            nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
            in_config.pull = NRF_GPIO_PIN_PULLUP;

            //Configuring 
            error::error_status err = nrfx_gpiote_in_init(PIN, &in_config, handler);

            if (!err)
                nrfx_gpiote_in_event_enable(PIN, true);
        }
        ~singleton_gpiote_config() {}
    };
}