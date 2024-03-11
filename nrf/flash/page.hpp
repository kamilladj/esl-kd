#pragma once

#include "nrf_fstorage.h"
#include "nrf_fstorage_nvmc.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_backend_usb.h"

#include "nrf_assert.h"
#include "nrf_dfu_types.h"

#include "error_status.hpp"
#include "static_vector.hpp"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

namespace nrf
{
    template<size_t header_size, size_t record_size>
    class page
    {
    public:

        page(uint32_t start_addr = 0, uint32_t end_addr = 0)
            : m_start_addr{start_addr}
            , m_end_addr{end_addr}
            , m_cur_pos{start_addr}
        {
            fstorage_init();
            page_erase();
            find_cur_pos();
        }

    private:

        void find_cur_pos()
        {
            uint32_t last_record_addr = m_start_addr + record_size * 5; //6-1

            for (m_cur_pos = last_record_addr; m_cur_pos >= m_start_addr; m_cur_pos -= record_size)
            {
                if (byte_read() != 0xFF)
                    return;
            }
        }

        void update_cur_pos()
        {
            m_cur_pos += record_size;
        }

        bool is_address_valid()
        {
            if (m_cur_pos >= m_start_addr && m_cur_pos <= m_end_addr)
                return true;
            else
                return false;
        }

        /*bool is_record_writable(static_vector<uint8_t, record_size>& buff)
        {
            for (size_t i = 0; i < record_size; i++)
            {
                if (buff[i] == 0xFF)
                    return false;
            }

            return true;
        }*/

    private:

        static void fstorage_evt_handler(nrf_fstorage_evt_t* p_evt)
        {
            error::error_status e = p_evt->result;

            if (!e)
            {
                if (p_evt->id == NRF_FSTORAGE_EVT_WRITE_RESULT)
                {
                    NRF_LOG_INFO("--> Event received: wrote %d bytes at address 0x%x.", p_evt->len, p_evt->addr);
                }
                else if (p_evt->id == NRF_FSTORAGE_EVT_ERASE_RESULT)
                {
                    NRF_LOG_INFO("--> Event received: erased %d page from address 0x%x.", p_evt->len, p_evt->addr);
                }
            }
        }

        error::error_status fstorage_init()
        {
            m_fstorage.evt_handler = fstorage_evt_handler;
            m_fstorage.start_addr = m_start_addr;
            m_fstorage.end_addr = m_end_addr;

            return nrf_fstorage_init(&m_fstorage, &nrf_fstorage_nvmc, NULL);
        }

        void wait_for_flash_ready()
        {
            while (nrf_fstorage_is_busy(&m_fstorage))
                __WFE();
        }

    private:

        uint8_t byte_read()
        {
            uint8_t res;

            NRFX_ASSERT(is_address_valid());

            error::error_status e = nrf_fstorage_read(&m_fstorage, m_cur_pos, &res, sizeof(res));

            if (!e)
                wait_for_flash_ready();

            return res;
        }

    public:

        /*bool is_page_empty()
        {
            return true;
        }

        bool is_page_full()
        {
            return true;
        }*/

    public:

        /*void read_page_header(static_vector<uint8_t, header_size>& buff)
        {
            NRFX_ASSERT(is_address_valid());

            error::error_status e = nrf_fstorage_read(&m_fstorage, m_start_addr, &buff[0], header_size);

            if (!e)
                wait_for_flash_ready();
        }

        void write_page_header(const static_vector<uint8_t, header_size>& buff)
        {
            NRFX_ASSERT(is_address_valid());

            error::error_status e = nrf_fstorage_write(&m_fstorage, m_start_addr, &buff[0], header_size, NULL);

            if (!e)
                wait_for_flash_ready();
        }*/

    public:

        void read_last_record(static_vector<uint8_t, record_size>& buff)
        {
            NRFX_ASSERT(is_address_valid());

            error::error_status e = nrf_fstorage_read(&m_fstorage, m_cur_pos, &buff[0], record_size);

            if (!e)
                wait_for_flash_ready();
        }

        void write_new_record(const static_vector<uint8_t, record_size>& buff)
        {
            //page_erase();

            NRFX_ASSERT(is_address_valid());

            error::error_status e = nrf_fstorage_write(&m_fstorage, m_cur_pos + record_size, &buff[0], record_size, NULL);

            if (!e)
                wait_for_flash_ready();

            update_cur_pos();
        }

    public:

        void page_erase()
        {
            m_cur_pos = m_start_addr;

            NRF_LOG_INFO("page_erase(%x)", m_cur_pos);

            error::error_status e = nrf_fstorage_erase(&m_fstorage, m_cur_pos, 1, NULL);

            if (!e)
                wait_for_flash_ready();
        }

    private:

        uint32_t              m_start_addr;
        uint32_t              m_end_addr;
        uint32_t              m_cur_pos;
        nrf_fstorage_t        m_fstorage;
    };
}