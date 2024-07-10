#pragma once

#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include "ble_gatts.h"
#include "app_util.h"

namespace nrf
{
    enum directions { up, down };

    class hsv
    {
    public:

        hsv(unsigned int device_id)
            : m_hue{uint16_t((device_id%100)*255/100)}
            , m_sat{255}
            , m_val{255}
        {}

        hsv(uint16_t h, uint16_t s, uint16_t v)
            : m_hue{h}
            , m_sat{s}
            , m_val{v}
        {}

        hsv(const ble_gatts_evt_write_t& write)
            : m_hue{ uint16_big_decode(&write.data[0]) }
            , m_sat{ uint16_big_decode(&write.data[2]) }
            , m_val{ uint16_big_decode(&write.data[4]) }
        {}

    public:

        void update_hue()
        {
            NRF_LOG_INFO("HUE");

            m_hue += 1;
            if (m_hue >= 255)
                m_hue = 0;
        }

        void update_saturation()
        {
            NRF_LOG_INFO("SAT");

            static directions dir = up;

            if (dir == up)
            {
                m_sat += 1;
                if (m_sat >= 255)
                    dir = down;
            }
            else
            {
                m_sat -= 1;
                if (m_sat == 0)
                    dir = up;
            }
        }

        void update_value()
        {
            NRF_LOG_INFO("VAL");

            static directions dir = up;

            if (dir == up)
            {
                m_val += 1;
                if (m_val >= 255)
                    dir = down;
            }
            else
            {
                m_val -= 1;
                if (m_val == 0)
                    dir = up;
            }
        }

    public:

        uint16_t get_hue() const
        {
            return m_hue;
        }

        uint16_t get_sat() const
        {
            return m_sat;
        }

        uint16_t get_val() const
        {
            return m_val;
        }

    public:

        void fill_data(uint8_t* data)
        {
            data[0] = (m_hue >> 8) & 0xFF;
            data[1] = m_hue & 0xFF;
            data[2] = (m_sat >> 8) & 0xFF;
            data[3] = m_sat & 0xFF;
            data[4] = (m_val >> 8) & 0xFF;
            data[5] = m_val & 0xFF;
        }

    private:

        uint16_t m_hue; //in degrees
        uint16_t m_sat;
        uint16_t m_val;
    };
}