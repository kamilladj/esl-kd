#pragma once

#include "nrfx_nvmc.h"

#include "error_status.hpp"

#include <stdint.h>
#include <stdbool.h>

namespace nrf
{
    class nvmc
    {
    public:

        nvmc()
            : m_address{ 0x000DD000 }
        {}

    private:

        error::error_status page_erase()
        {
            return nrfx_nvmc_page_erase(m_address);
        }

        bool word_writable_check(uint32_t val)
        {
            return nrfx_nvmc_word_writable_check(m_address, val);
        }

        void word_write(uint32_t val)
        {
            nrfx_nvmc_word_write(m_address, val);
        }

        bool write_done_check()
        {
            return nrfx_nvmc_write_done_check();
        }

    public:

        bool is_empty()
        {
            uint32_t* ptr = (uint32_t*)m_address;

            return *ptr == 0xFFFFFFFF;
        }

    public:

        void save_data(uint32_t val)
        {
            error::error_status err;
            if (!word_writable_check(val))
                err = page_erase();

            if (!err)
                word_write(val);

            while (!write_done_check());
        }

        uint32_t get_data()
        {
            uint32_t* ptr = (uint32_t*)m_address;

            if (is_empty())
                return 0;
            else
                return *ptr;
        }

    private:

        const uint32_t m_address;
    };
}