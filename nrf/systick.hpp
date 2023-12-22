#pragma once

#include "nrfx_systick.h"
#include "nrf/singleton_systick.hpp"

#include <stdint.h>

namespace nrf
{
    class systick
    {
    private:

        systick(const systick&);
        systick& operator=(const systick&);

    public:

        systick()
            : m_state{}
        {
            singleton_systick::init();
        }

    public:

        void get()
        {
            nrfx_systick_get(&m_state);
        }

        bool test(uint32_t us)
        {
            return nrfx_systick_test(&m_state, us);
        }

    private:

        nrfx_systick_state_t m_state;
    };
}