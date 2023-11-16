#pragma once

#include "nrf_mtx.h"
#include "nrf_assert.h"


class lock_guard
{
public:

    explicit lock_guard(nrf_mtx_t& m)
        :m_mtx(m)
    {
        ASSERT(nrf_mtx_trylock(&m_mtx));
    }

    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;

    ~lock_guard()
    {
        nrf_mtx_unlock(&m_mtx);
    }

private:

    nrf_mtx_t& m_mtx;
};