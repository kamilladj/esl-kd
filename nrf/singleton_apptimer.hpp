#pragma once

#include "app_timer.h"
#include "nrf_drv_clock.h"

namespace nrf
{
    class singleton_apptimer
    {
        public:
            static singleton_apptimer& instance()
            {
                static singleton_apptimer s;
                return s;
            }

            singleton_apptimer(const singleton_apptimer&) = delete;
            singleton_apptimer& operator=(const singleton_apptimer&) = delete;

        private:
            singleton_apptimer() 
            {
                nrf_drv_clock_init();
                nrf_drv_clock_lfclk_request(NULL);
                app_timer_init();
            }
            ~singleton_apptimer() {}
    };
}