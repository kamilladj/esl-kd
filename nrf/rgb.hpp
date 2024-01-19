#pragma once

#include <stdint.h>

namespace nrf
{
    class rgb
    {
    public:

        rgb()
            : m_red{0}
            , m_green{0}
            , m_blue{0}
        {}

    public:

        void update_values(uint16_t r, uint16_t g, uint16_t b)
        {
            m_red = r;
            m_green = g;
            m_blue = b;
        }

    public:

        uint16_t get_red()
        {
            return m_red;
        }

        uint16_t get_green()
        {
            return m_green;
        }

        uint16_t get_blue()
        {
            return m_blue;
        }

    private:

        uint16_t m_red;
        uint16_t m_green;
        uint16_t m_blue;
    };
}