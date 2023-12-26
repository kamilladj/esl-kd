#pragma once

#include "nrfx_systick.h"

namespace nrf
{
    class singleton_systick
    {
    public:

        static singleton_systick& init()
        {
            static singleton_systick s;
            return s;
        }

        singleton_systick(const singleton_systick&) = delete;
        singleton_systick& operator=(const singleton_systick&) = delete;

    private:

        singleton_systick()
        {
            nrfx_systick_init();
        }
        ~singleton_systick() {}
    };
}