#pragma once

#include "rgb.hpp"

#include <stdint.h>

#define ID 4231

namespace nrf
{
    class hsv
    {
    public:

        hsv()
            : m_hue{79}
            , m_sat{255}
            , m_val{255}
            , m_step_value{1}
        {}

    public:

        void update_hue()
        {
            m_hue += m_step_value;

            if (m_hue == 0 || m_hue == 255)
                m_step_value *= -1;
        }

        void update_saturation()
        {
            m_sat += m_step_value;

            if (m_sat == 0 || m_sat == 255)
                m_step_value *= -1;
        }

        void update_value()
        {
            m_val += m_step_value;

            if (m_val == 0 || m_val == 255)
                m_step_value *= -1;
        }

    public:

        void hsv2rgb(rgb& color)
        {
            uint16_t region, remainder, p, q, t;

            if (m_sat == 0)
            {
                color.update_values(m_val, m_val, m_val);

                return;
            }

            region = m_hue / 43;
            remainder = (m_hue - (region * 43)) * 6;

            p = (m_val * (255 - m_sat)) >> 8;
            q = (m_val * (255 - ((m_sat * remainder) >> 8))) >> 8;
            t = (m_val * (255 - ((m_sat * (255 - remainder)) >> 8))) >> 8;

            switch (region)
            {
            case 0:
                color.update_values(m_val, t, p);
                break;
            case 1:
                color.update_values(q, m_val, p);
                break;
            case 2:
                color.update_values(p, m_val, t);
                break;
            case 3:
                color.update_values(p, q, m_val);
                break;
            case 4:
                color.update_values(t, p, m_val);
                break;
            default:
                color.update_values(m_val, p, q);
                break;
            }
        }

    private:

        uint16_t    m_hue; //degrees
        uint16_t    m_sat;
        uint16_t    m_val;
        int16_t     m_step_value;
    };
}