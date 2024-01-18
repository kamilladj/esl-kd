#pragma once

#include "utils/static_function.hpp"
#include "error/error_status.hpp"
#include "nrfx_pwm.h"

#include "utils/static_vector.hpp"

#include "lock_guard.hpp"
#include "mutex.hpp"

#include <stdint.h>

#include "hsv.hpp"
#include "rgb.hpp"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

enum blink_modes { default_mode, hue_mode, saturation_mode, value_mode };

namespace nrf
{
    template<uint16_t id>
    class pwm
    {
    public:

        pwm(utils::static_function<void()> handler)
            : m_blink_mode{ default_mode }
            , m_step_value{ 1 }
            , m_pwm{}
            , m_seq_values{}
            , m_seq{}
            , m_hsv{}
            , m_rgb{}
        {
            nrfx_pwm_config_t config = NRFX_PWM_DEFAULT_CONFIG;
            error::error_status err = nrfx_pwm_init(&m_pwm, &config, static_handler);

            m_seq.values.p_individual = &m_seq_values;
            m_seq.length = NRF_PWM_VALUES_LENGTH(m_seq_values);
            m_seq.repeats = 0;
            m_seq.end_delay = 0;

            if (!err)
            {
                NRF_LOG_INFO("Playback started");
                nrfx_pwm_simple_playback(&m_pwm, &m_seq, PWM_PLAYBACK_COUNT, NRFX_PWM_FLAG_LOOP);
            }

            lock_guard lk(m_mtx);
            m_handlers.push_back(handler);
        }

    public:

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
            m_seq_values.channel_0 += m_step_value;

            if (m_seq_values.channel_0 == 0 || m_seq_values.channel_0 == 255)
                m_step_value *= -1;
        }


        void update_led2()
        {
            m_hsv.hsv2rgb(m_rgb);
            m_seq_values.channel_1 = m_rgb.get_red();
            m_seq_values.channel_2 = m_rgb.get_green();
            m_seq_values.channel_3 = m_rgb.get_blue();
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
            NRF_LOG_INFO("Double click handler");
            change_mode();

            m_seq_values.channel_0 = 0;

            if (m_blink_mode == hue_mode)
            {
                m_step_value = 5;
            }
            else if (m_blink_mode == saturation_mode)
            {
                m_step_value = 20;
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

        typedef static_vector<utils::static_function<void()>, 5> handlers_vector;

    private:

        atomic_32                   m_blink_mode;
        int16_t                     m_step_value;
        nrfx_pwm_t                  m_pwm;
        nrf_pwm_values_individual_t m_seq_values;
        nrf_pwm_sequence_t          m_seq;
        hsv                         m_hsv;
        rgb                         m_rgb;
        static mutex                m_mtx;
        static handlers_vector      m_handlers;
    };

    template<uint16_t id>
    mutex pwm<id>::m_mtx;

    template<uint16_t id>
    static_vector<utils::static_function<void()>, 5> pwm<id>::m_handlers;
}