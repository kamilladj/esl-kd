#pragma once

#include "nrf_mtx.h"

namespace nrf
{
    class mutex
    {
    public:

        mutex()
        {
            nrf_mtx_init(&m);
        }

        mutex(const mutex&) = delete;
        mutex& operator=(const mutex&) = delete;

        ~mutex()
        {
            nrf_mtx_destroy(&m);
        }

    public:

        bool try_lock()
        {
            return nrf_mtx_trylock(&m);
        }

        void unlock()
        {
            nrf_mtx_unlock(&m);
        }

    private:

        nrf_mtx_t m;
    };
}