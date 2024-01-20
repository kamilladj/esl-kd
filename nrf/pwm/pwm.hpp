#pragma once

#include "static_function.hpp"
#include "error_status.hpp"
#include "nrfx_pwm.h"
#include "static_vector.hpp"

#include "lock_guard.hpp"
#include "mutex.hpp"

#include <stdint.h>

#include "hsv.hpp"
#include "rgb.hpp"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

namespace nrf
{
    template<uint16_t id>
    class pwm
    {
    public:

        pwm(const unsigned int& device_id, utils::static_function<void()> handler)
            : m_blink_mode{ default_mode }
            , m_direction{ up }
            , m_step_value{ 0 }
            , m_pwm_instance{}
            , m_seq_values{}
            , m_hsv{ device_id }
        {
            error::error_status err = init();

            {
                lock_guard lk(m_mtx);
                m_handlers.push_back(handler);
            }

            if (!err)
                playback();
        }

    private:

        error::error_status init()
        {
            m_pwm_instance = NRFX_PWM_INSTANCE(0);
            nrfx_pwm_config_t config = NRFX_PWM_DEFAULT_CONFIG;

            return nrfx_pwm_init(&m_pwm_instance, &config, static_handler);
        }

    private:

        void playback()
        {
            nrf_pwm_sequence_t seq;
            seq.values.p_individual = &m_seq_values;
            seq.length = NRF_PWM_VALUES_LENGTH(m_seq_values);

            const uint16_t playback_count = 1;

            nrfx_pwm_simple_playback(&m_pwm_instance, &seq, playback_count, NRFX_PWM_FLAG_LOOP);
        }

    private:

        void change_mode()
        {
            NRF_LOG_INFO("Change mode");

            if (m_blink_mode == default_mode)
                m_blink_mode = hue_mode;
            else if (m_blink_mode == hue_mode)
                m_blink_mode = saturation_mode;
            else if (m_blink_mode == saturation_mode)
                m_blink_mode = value_mode;
            else
                m_blink_mode = default_mode;
        }

    public:

        void update_led1()
        {
            if (m_direction == up)
            {
                m_seq_values.channel_0 += m_step_value;
                if (m_seq_values.channel_0 >= 255)
                    m_direction = down;
            }
            else
            {
                m_seq_values.channel_0 -= m_step_value;
                if (m_seq_values.channel_0 <= 0)
                    m_direction = up;
            }
        }


        void update_led2()
        {
            rgb color(m_hsv);
            m_seq_values.channel_1 = color.red;
            m_seq_values.channel_2 = color.green;
            m_seq_values.channel_3 = color.blue;
        }


        void update_hsv()
        {
            NRF_LOG_INFO("Update hsv");

            if (m_blink_mode == hue_mode)
                m_hsv.update_hue();
            else if (m_blink_mode == saturation_mode)
                m_hsv.update_saturation();
            else if (m_blink_mode == value_mode)
                m_hsv.update_value();
            else
                return;
        }

    public:

        void double_click_handler()
        {
            change_mode();

            m_seq_values.channel_0 = 0;
            m_direction = up;

            if (m_blink_mode == hue_mode)
            {
                m_step_value = 5;
            }
            else if (m_blink_mode == saturation_mode)
            {
                m_step_value = 25;
            }
            else if (m_blink_mode == value_mode)
            {
                m_seq_values.channel_0 = 255;
                m_step_value = 0;
            }
            else
            {
                m_step_value = 0;
            }
        }

    private:

        static void static_handler(nrfx_pwm_evt_type_t event_type)
        {
            handlers_vector tmp;

            {
                lock_guard lk(m_mtx);
                tmp = m_handlers;
            }

            for (size_t i = 0; i < tmp.get_size(); i++)
                tmp[i]();
        }

    private:

        enum { default_mode, hue_mode, saturation_mode, value_mode };
        typedef static_vector<utils::static_function<void()>, 5> handlers_vector;

    private:

        atomic_32                   m_blink_mode;
        atomic_32                   m_direction;
        uint16_t                    m_step_value;
        nrfx_pwm_t                  m_pwm_instance;
        nrf_pwm_values_individual_t m_seq_values;
        hsv                         m_hsv;
        static mutex                m_mtx;
        static handlers_vector      m_handlers;
    };

    template<uint16_t id>
    mutex pwm<id>::m_mtx;

    template<uint16_t id>
    static_vector<utils::static_function<void()>, 5> pwm<id>::m_handlers;
}