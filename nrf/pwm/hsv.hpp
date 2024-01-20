#pragma once

#include <stdint.h>

namespace nrf
{
    enum directions { up, down };

    class hsv
    {
    public:

        hsv(const unsigned int& device_id)
            : m_hue{uint16_t((device_id%100)*255/100)}
            , m_sat{255}
            , m_val{255}
            , m_step_value{1}
        {}

    public:

        void update_hue()
        {
            static directions dir = up;

            if (dir == up)
            {
                m_hue += m_step_value;
                if (m_hue >= 255)
                    dir = down;
            }
            else
            {
                m_hue -= m_step_value;
                if (m_hue <= 0)
                    dir = up;
            }
        }

        void update_saturation()
        {
            static directions dir = up;

            if (dir == up)
            {
                m_sat += m_step_value;
                if (m_sat >= 255)
                    dir = down;
            }
            else
            {
                m_sat -= m_step_value;
                if (m_sat <= 0)
                    dir = up;
            }
        }

        void update_value()
        {
            static directions dir = up;

            if (dir == up)
            {
                m_val += m_step_value;
                if (m_val >= 255)
                    dir = down;
            }
            else
            {
                m_val -= m_step_value;
                if (m_val <= 0)
                    dir = up;
            }
        }

    public:

        uint16_t get_hue()
        {
            return m_hue;
        }

        uint16_t get_sat()
        {
            return m_sat;
        }

        uint16_t get_val()
        {
            return m_val;
        }

    private:

        uint16_t m_hue; //in degrees
        uint16_t m_sat;
        uint16_t m_val;
        uint16_t m_step_value;
    };
}