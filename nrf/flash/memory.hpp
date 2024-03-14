#pragma once

#include "page.hpp"
#include "static_vector.hpp"

#include "nrf_assert.h"
#include "nrf_dfu_types.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

namespace nrf
{
    template<size_t size>
    class memory
    {
    public:

        memory()
            : m_start_addr{ BOOTLOADER_ADDRESS - NRF_DFU_APP_DATA_AREA_SIZE }
            , m_end_addr{ BOOTLOADER_ADDRESS - 1}
            , m_pages{ page<m_page_header_size, size>(m_start_addr, m_start_addr + CODE_PAGE_SIZE - 1)
                     , page<m_page_header_size, size>(m_start_addr + CODE_PAGE_SIZE, m_start_addr + 2 * CODE_PAGE_SIZE - 1)
                     , page<m_page_header_size, size>(m_start_addr + 2 * CODE_PAGE_SIZE, m_end_addr) }
            , m_cur_page{ 0 }
        {
            NRFX_ASSERT(m_num_of_pages >= 2);
            //write_page_headers();
        }

    private:

        //bool validate_page_header()
        //{
        //    read_page_header(i);

        //    for (size_t i = 0; i < m_num_of_pages; i++)
        //        read_page_headers(i, 0);
        //}

        //void write_page_headers()
        //{
        //    for (size_t i = 0; i < m_num_of_pages; i++)
        //        write_page_header(i, 0);
        //}

        //void write_page_header(size_t i, uint8_t idx)
        //{
        //    static_vector<uint8_t, m_page_header_size> vec;

        //    vec.push_back(0);
        //    vec.push_back(255);
        //    vec.push_back(250); //65530
        //    vec.push_back(1);
        //    vec.push_back(119); // version 375
        //    vec.push_back(idx); //index
        //    vec.push_back(0);
        //    vec.push_back(7); //crc 111
        //    vec.push_back(0);
        //    vec.push_back(0); //reserved

        //    m_pages[i].write_page_header(vec);

        //    cnt++;
        //}

        //void rewrite_page_header()
        //{
        //    static uint8_t cnt = 1;

        //    write_page_header(m_cur_page, cnt);

        //    cnt++;
        //}

    private:

        bool is_record_writable(const static_vector<uint8_t, size>& buff)
        {
            for (size_t i = 0; i < size; i++)
            {
                if (buff[i] == 0xFF)
                    return false;
            }

            return true;
        }

    public:

        void read_from_page(static_vector<uint8_t, size>& buff)
        {
            //if (!m_pages[m_cur_page].is_page_empty())
                m_pages[m_cur_page].read_last_record(buff);
            //else
                //return;
        }

        void write_to_page(const static_vector<uint8_t, size>& buff)
        {
            if (!(is_record_writable(buff)))
                return;

            /*if (m_pages[m_cur_page].is_page_full())
            {
                m_cur_page++;
                m_cur_page %= m_num_of_pages;
                m_pages[m_cur_page].page_erase();
                rewrite_page_header();
            }*/

            m_pages[m_cur_page].write_new_record(buff);
        }
        
    private:

        uint32_t                       m_start_addr;
        uint32_t                       m_end_addr;
        static const uint32_t          m_page_header_size = 10;
        static const uint32_t          m_num_of_pages = 3;
        page<m_page_header_size, size> m_pages[m_num_of_pages];
        uint8_t                        m_cur_page;
    };

    template<size_t size>
    const uint32_t memory<size>::m_page_header_size;

    template<size_t size>
    const uint32_t memory<size>::m_num_of_pages;
}