#pragma once

#include "nrf_drv_gpiote.h"
#include "utils/static_function.hpp"
#include "error/error_status.hpp"
#include "singleton_gpiote.hpp"
#include "singleton_gpiote_config.hpp"
#include "utils/static_vector.hpp"

#include "lock_guard.hpp"
#include "mutex.hpp"

#include <stddef.h>

namespace nrf
{
    template<nrfx_gpiote_pin_t PIN>
    class async_button
    {
    public:

        async_button(utils::static_function<void(error::error_status)> handler)
        {
            singleton_gpiote::init();
            singleton_gpiote_config<PIN>::init(button_handler);

            lock_guard lk(m_mtx);
            m_handlers.push_back(handler);
        }

    private:

        static void button_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
        {
            handlers_vector tmp;

            {
                lock_guard lk(m_mtx);
                tmp = m_handlers;
            }

            for (size_t i = 0; i < tmp.get_size(); i++)
                tmp[i](error::error_status());
        }

    private:

        typedef static_vector<utils::static_function<void(error::error_status)>, 5> handlers_vector;

    private:

        static mutex           m_mtx;
        static handlers_vector m_handlers;
    };

    template<nrfx_gpiote_pin_t PIN>
    mutex async_button<PIN>::m_mtx;

    template<nrfx_gpiote_pin_t PIN>
    static_vector<utils::static_function<void(error::error_status)>, 5> async_button<PIN>::m_handlers;
}