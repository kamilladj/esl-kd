#pragma once

#include "app_scheduler.h"
#include "app_error.h"
#include "mem_manager.h"

#include "static_function.hpp"
#include "error_status.hpp"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include <stdint.h>
#include <stdlib.h>
#include <new>

namespace nrf
{
    template<uint16_t max_event_size, uint16_t queue_size = 10>
    class scheduler
    {
    private:

        scheduler(const scheduler&);
        scheduler& operator=(const scheduler&);

    public:

        scheduler()
        {
            APP_SCHED_INIT(max_event_size, queue_size);
        }

    public:

        error::error_status put_event(utils::static_function<void()> handler)
        {
            utils::static_function<void()>* p = (utils::static_function<void()>*)nrf_malloc(sizeof(handler));

            NRF_LOG_INFO("Address: %p", p);

            error::error_status e;

            if (p == nullptr)
            {
                NRF_LOG_INFO("Memory allocation failed");
                e = NRF_ERROR_NO_MEM;
            }
            else
            {
                new (p) utils::static_function<void()>();
                *p = handler;

                e = app_sched_event_put(&p, sizeof(p), static_handler);

                if (e)
                {
                    NRF_LOG_INFO("Couldn't put event into scheduler");
                    p->~static_function<void()>();
                    nrf_free(p);
                }
                else
                {
                    NRF_LOG_INFO("Event put into scheduler");
                }
            }

            return e;
        }

    public:

        void execute()
        {
            app_sched_execute();
        }

    private:

        static void static_handler(void* p_context, uint16_t event_size)
        {
            utils::static_function<void()>** ptr = (utils::static_function<void()>**)p_context;

            NRF_LOG_INFO("Address: %p", *ptr);

            (**ptr)();

            (**ptr).~static_function<void()>();

            nrf_free(*ptr);
        }
    };

}   // namespace nrf
