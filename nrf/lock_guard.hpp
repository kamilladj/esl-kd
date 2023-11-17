#pragma once

#include "mutex.hpp"
#include "nrf_assert.h"

namespace nrf
{
    class lock_guard
    {
    public:

        explicit lock_guard(mutex& m)
            :m_mtx(m)
        {
            ASSERT(m_mtx.try_lock());
        }

        lock_guard(const lock_guard&) = delete;
        lock_guard& operator=(const lock_guard&) = delete;

        ~lock_guard()
        {
            m_mtx.unlock();
        }

    private:

        mutex& m_mtx;
    };
}