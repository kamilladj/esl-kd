#pragma once

#include "nrfx_nvmc.h"
#include "app_util.h"
#include "nrf_dfu_types.h"

#include "error_status.hpp"

#include <stdint.h>

namespace nrf
{
    class nvmc
    {
    public:

        nvmc()
            : m_start_address{ BOOTLOADER_ADDRESS - NRF_DFU_APP_DATA_AREA_SIZE }
            , m_num_words{ 3 }
        {}

    public:

        bool is_empty()
        {
            uint32_t* p_address = (uint32_t*)m_start_address;

            bool empty = true;

            for (uint32_t i = 0; i < m_num_words; i++)
            {
                if (*(p_address + i) != 0xFFFFFFFF)
                {
                    empty = false;
                    break;
                }
            }

            return empty;
        }

    public:

        void write_data(uint32_t const* src)
        {
            error::error_status err;

            for (uint32_t i = 0; i < m_num_words; i++)
            {
                if (!nrfx_nvmc_word_writable_check(m_start_address + i, *(src + i)))
                {
                    err = nrfx_nvmc_page_erase(m_start_address);
                    break;
                }
            }

            if (!err)
                nrfx_nvmc_words_write(m_start_address, src, m_num_words);

            while (!nrfx_nvmc_write_done_check());
        }

        void read_data(uint32_t* dest)
        {
            uint32_t* p_address = (uint32_t*)m_start_address;

            for (uint32_t i = 0; i < m_num_words; i++)
                *(dest + i) = *(p_address + i);
        }

    private:

        const uint32_t m_start_address;
        uint32_t m_num_words;
    };
}