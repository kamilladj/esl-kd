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
    template<size_t record_size, size_t header_size>
    class page
    {
    public:

        page(uint32_t start_addr = 0, uint32_t end_addr = 0)
            : m_start_addr{start_addr}
            , m_end_addr{end_addr}
            , m_cur_pos{start_addr}                     
        {
            fstorage_init();
        }

    public:

        uint32_t get_index(size_t offset)
        {
            return read_byte(m_start_addr + offset);
        }

    public:

        bool is_empty()
        {
            return m_cur_pos < m_start_addr + header_size;

        }

        bool is_full()
        {
            return m_cur_pos + 2 * record_size > m_end_addr;
        }

    public:

        void update_cur_pos()
        {
            uint32_t first_record_addr = m_start_addr + header_size;
            uint32_t max_num_of_records = (CODE_PAGE_SIZE - header_size) / record_size;
            uint32_t last_record_addr = first_record_addr + record_size * (max_num_of_records - 1);

            for (m_cur_pos = last_record_addr; m_cur_pos >= first_record_addr; m_cur_pos -= record_size)
            {
                if (read_byte(m_cur_pos) != 0xFF)
                    return;
            }

            m_cur_pos = 0;
        }

        uint32_t get_cur_pos() const
        {
            return m_cur_pos;
        }

    private:

        bool is_address_valid(uint32_t addr)
        {
            if (addr >= m_start_addr && addr <= m_end_addr)
                return true;
            else
                return false;
        }

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
            else
            {
                NRF_LOG_INFO("--> Event received: ERROR while executing an fstorage operation.");
            }
        }

        error::error_status fstorage_init()
        {
            m_fstorage.evt_handler = fstorage_evt_handler;
            m_fstorage.start_addr = m_start_addr;
            m_fstorage.end_addr = m_end_addr;

            return nrf_fstorage_init(&m_fstorage, &nrf_fstorage_nvmc, NULL);
        }

    private:

        void wait_for_flash_ready()
        {
            while (nrf_fstorage_is_busy(&m_fstorage))
                __WFE();
        }

    public:

        uint8_t read_byte(uint32_t addr)
        {
            uint8_t res;

            NRFX_ASSERT(is_address_valid(addr));

            error::error_status e = nrf_fstorage_read(&m_fstorage, addr, &res, sizeof(res));

            if (!e)
                wait_for_flash_ready();

            return res;
        }

    public:

        void read_page_header(static_vector<uint8_t, header_size>& buff)
        {
            error::error_status e = nrf_fstorage_read(&m_fstorage, m_start_addr, &buff[0], header_size);

            if (!e)
                wait_for_flash_ready();
        }

        void write_page_header(const static_vector<uint8_t, header_size>& buff)
        {
            error::error_status e = nrf_fstorage_write(&m_fstorage, m_start_addr, &buff[0], header_size, NULL);

            if (!e)
                wait_for_flash_ready();
        }

    public:

        void read_last_record(static_vector<uint8_t, record_size>& buff)
        {
            NRFX_ASSERT(is_address_valid(m_cur_pos));

            error::error_status e = nrf_fstorage_read(&m_fstorage, m_cur_pos, &buff[0], record_size);

            if (!e)
                wait_for_flash_ready();
        }

        void write_new_record(const static_vector<uint8_t, record_size>& buff)
        {                     
            if (m_cur_pos == 0)
                m_cur_pos = m_start_addr + header_size;

            NRFX_ASSERT(is_address_valid(m_cur_pos));

            error::error_status e = nrf_fstorage_write(&m_fstorage, m_cur_pos + record_size, &buff[0], record_size, NULL);

            if (!e)
                wait_for_flash_ready();

            m_cur_pos += record_size;
        }

    public:

        void erase()
        {
            m_cur_pos = m_start_addr;

            NRF_LOG_INFO("page_erase(%x)", m_cur_pos);

            error::error_status e = nrf_fstorage_erase(&m_fstorage, m_cur_pos, 1, NULL);

            if (!e)
                wait_for_flash_ready();
        }

    private:

        uint32_t       m_start_addr;
        uint32_t       m_end_addr;
        uint32_t       m_cur_pos;
        nrf_fstorage_t m_fstorage;
    };
}
