#pragma once

#include <stdint.h>
#include <stddef.h>

#include "nrf/atomic_32.hpp"

namespace nrf
{
    class pwm
    {
    public:

        pwm()
            : m_frequency{ 1000 }
            , m_period_ms{ 1 / m_frequency * 1000 }
            , m_step_value{ 1 }
            , m_duty_cycle{ 0 }
            , m_time_on_ms{ 0 }
            , m_time_off_ms{ 0 }
            , m_time_to_wait_ms{ 0 }
        {}

    public:

        uint32_t get_duty_cycle()
        {
            return m_duty_cycle;
        }

        uint32_t get_time_to_wait_ms()
        {
            return m_time_to_wait_ms;
        }

    public:

        void update_duty_cycle()
        {
            m_duty_cycle += m_step_value;

            if (m_duty_cycle == 0 || m_duty_cycle == 100)
                m_step_value = m_step_value * (-1);
        }

        void update_time_to_wait_ms(bool cond)
        {
            m_time_on_ms = m_period_ms * m_duty_cycle / 100;
            m_time_off_ms = m_period_ms - m_time_on_ms;

            m_time_to_wait_ms = cond ? m_time_on_ms : m_time_off_ms;
        }

    private:

        const uint32_t m_frequency;
        const uint32_t m_period_ms;
        int32_t        m_step_value;
        uint32_t       m_duty_cycle;
        uint32_t       m_time_on_ms;
        uint32_t       m_time_off_ms;
        uint32_t       m_time_to_wait_ms;
    };
}