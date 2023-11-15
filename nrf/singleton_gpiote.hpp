#pragma once

#include "nrf_drv_gpiote.h"

namespace nrf
{
    class singleton_gpiote
    {
    public:

        static singleton_gpiote& init()
        {
            static singleton_gpiote s;
            return s;
        }

        singleton_gpiote(const singleton_gpiote&) = delete;
        singleton_gpiote& operator=(const singleton_gpiote&) = delete;

    private:

        singleton_gpiote() 
        {
            nrfx_gpiote_init();
        }
        ~singleton_gpiote() {}
    };
}