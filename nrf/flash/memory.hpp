#pragma once

#include "page.hpp"
#include "static_vector.hpp"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_backend_usb.h"

#include "nrf_assert.h"
#include "nrf_dfu_types.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

namespace nrf
{
    template<size_t size>
    template<size_t record_size, size_t header_size = 8>
    class memory
    {
    public:

        memory()
            : m_start_addr{ BOOTLOADER_ADDRESS - NRF_DFU_APP_DATA_AREA_SIZE }
            , m_end_addr{ BOOTLOADER_ADDRESS - 1}
            , m_pages{ page<record_size, header_size>(m_start_addr, m_start_addr + CODE_PAGE_SIZE - 1)
                     , page<record_size, header_size>(m_start_addr + CODE_PAGE_SIZE, m_start_addr + 2 * CODE_PAGE_SIZE - 1)
                     , page<record_size, header_size>(m_start_addr + 2 * CODE_PAGE_SIZE, m_end_addr) }  
            , m_cur_page{ 0 }
        {
            NRFX_ASSERT(m_num_of_pages >= 2);
            check_pages_format();
            find_cur_page();
        }
        
    private:

        void check_pages_format()
        {
            for (size_t i = 0; i < m_num_of_pages; i++)
            {
                if (!is_page_header_valid(i))
                    edit_page(i, 0);

                m_pages[i].find_cur_pos();
            }

            NRFX_ASSERT(are_all_page_headers_valid());
        }

        void find_cur_page()
        {
            size_t max = m_pages[0].get_index(m_page_index_offset);

            for (size_t i = 0; i < m_num_of_pages; i++)
            {
                size_t index = m_pages[i].get_index(m_page_index_offset);

                if (index > max)
                {
                    max = index;
                    m_cur_page = i;
                }

                if (index == 100 && m_pages[i].is_full())
                {
                    m_cur_page++;
                    m_cur_page %= m_num_of_pages;
                    return;
                }
            }
        }

        void edit_page(size_t i, size_t index)
        {
            m_pages[i].erase();

            static_vector<uint8_t, header_size> buff;
            fill_page_header(index, buff);
            m_pages[i].write_page_header(buff);
        }

        void switch_to_next_page()
        {
            size_t index = m_pages[m_cur_page].get_index(m_page_index_offset);

            m_cur_page++;
            m_cur_page %= m_num_of_pages;

            size_t new_index = index++;
            if (new_index > 100)
                new_index = 0;

            edit_page(m_cur_page, new_index);
        }

    private:

        bool is_page_header_valid(size_t i)
        {
            static_vector<uint8_t, header_size> buff;

            m_pages[i].read_page_header(buff);

        }

        bool are_all_page_headers_valid()
        {
            for (size_t i = 0; i < m_num_of_pages; i++)
            {
                if (!is_page_header_valid(i))
                    return false;
            }

            return true;
        }

        void fill_page_header(uint8_t index, static_vector<uint8_t, header_size>& buff)
        {
            buff.push_back(255);
            buff.push_back(250); //id 65530
            buff.push_back(1);
            buff.push_back(119); //version 375
            buff.push_back(index); //index
            buff.push_back(26);
            buff.push_back(187); //crc 6843
            buff.push_back(0); //reserved
        }

    private:

        bool is_record_writable(const static_vector<uint8_t, record_size>& buff)
        {
            if (buff[0] == 0xFF)
                return false;

            return true;
        }

    public:

        bool read_record_from_page(static_vector<uint8_t, record_size>& buff)
        {
            if (m_pages[m_cur_page].is_empty())
                return false;
            else
                m_pages[m_cur_page].read_last_record(buff);

            return true;
        }

        void write_record_to_page(const static_vector<uint8_t, record_size>& buff)
        {
            if (!(is_record_writable(buff)))
            {
                NRF_LOG_INFO("Record not writtable");
                return;
            }

            if (m_pages[m_cur_page].is_full())
            {
                switch_to_next_page();
                m_pages[m_cur_page].find_cur_pos();
            }

            m_pages[m_cur_page].write_new_record(buff);
        }

    private:

        uint32_t                       m_start_addr;
        uint32_t                       m_end_addr;
        static const uint32_t          m_page_index_offset = 4;
        static const uint32_t          m_num_of_pages = NRF_DFU_APP_DATA_AREA_SIZE / CODE_PAGE_SIZE;
        page<record_size, header_size> m_pages[m_num_of_pages];
        uint8_t                        m_cur_page;
    };

    template<size_t record_size, size_t header_size>
    const uint32_t memory<record_size, header_size>::m_page_index_offset;

    template<size_t record_size, size_t header_size>
    const uint32_t memory<record_size, header_size>::m_num_of_pages;

}