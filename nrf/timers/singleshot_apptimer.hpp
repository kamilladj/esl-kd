#pragma once

#include "app_timer.h"
#include "static_function.hpp"
#include "error_status.hpp"
#include "singleton_apptimer.hpp"

namespace nrf
{
	class singleshot_apptimer
	{
	private:

	    singleshot_apptimer(const singleshot_apptimer&);
	    singleshot_apptimer& operator=(const singleshot_apptimer&);

	public:

		singleshot_apptimer()
			: m_app_timer{0}
			, m_p_app_timer{&m_app_timer}
			, m_handler{}
		{
            singleton_apptimer::init();
            app_timer_create(&m_p_app_timer, APP_TIMER_MODE_SINGLE_SHOT, static_handler);
        }

	public: 

		void async_wait(uint32_t time_ms, utils::static_function<void(error::error_status)> handler)
		{
            error::error_status err = app_timer_start(m_p_app_timer, APP_TIMER_TICKS(time_ms), this);
            if (err)
                handler(err);
            else
                m_handler = handler;
		}
	   
        error::error_status cancel()
        {
            error::error_status err = app_timer_stop(m_p_app_timer);
            return err;
        }

    private:

        static void static_handler(void* p_context)
        {
            singleshot_apptimer* ss = (singleshot_apptimer*)(p_context);
            (ss->m_handler)(error::error_status());
        }
	    
	private:

		app_timer_t                                       m_app_timer;
		app_timer_t*                                      m_p_app_timer;
		utils::static_function<void(error::error_status)> m_handler;
	};
}

