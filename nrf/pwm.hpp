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
            : m_period_us{ 1000 }
            , m_step_value{ 1 }
            , m_duty_cycle{ 0 }
        {}

    public:

        uint32_t get_duty_cycle()
        {
            return m_duty_cycle;
        }

        uint32_t get_time_on_us()
        {
            return m_period_us * get_duty_cycle() / 100;
        }

        uint32_t get_time_off_us()
        {
            return m_period_us - get_time_on_us();
        }

    public:

        void change_duty_cycle()
        {
            m_duty_cycle += m_step_value;

            if (m_duty_cycle == 0 || m_duty_cycle == 100)
                m_step_value *= -1;
        }

    private:

        const uint32_t m_period_us;
        int32_t        m_step_value;
        uint32_t       m_duty_cycle;
    };
}