#pragma once

#include <stdint.h>
#include "nrf_atomic.h"

namespace nrf
{

class atomic_32
{
    atomic_32(const atomic_32& rhs) = delete;
    atomic_32& operator=(const atomic_32& rhs) = delete;

public:

    using value_type = uint32_t;

    atomic_32()
        : m_value(0)
    {}

    explicit atomic_32(value_type value) 
        : m_value(value) 
    {}

public:

    /**
     * Atomically loads and returns the current value of the atomic variable
     *
     * @return The current value of the atomic variable
     */
    value_type load() const 
    {
        return m_value;
    }

    /**
     * Stores value to an atomic object
     *
     * @return The current value of the atomic variable
     */
    value_type store(value_type new_value) 
    {
        return nrf_atomic_u32_store(&m_value, new_value);
    }

    /**
     * Atomically replaces the underlying value with the new one
     *
     * @return The value of the atomic variable before the call
     */
    value_type exchange(value_type new_value)
    {
        return nrf_atomic_u32_fetch_store(&m_value, new_value);
    }

public:

    value_type operator=(value_type new_value) 
    {
        return store(new_value);
    }

    operator value_type() const
    {
        return load();
    }

public:

    /**
     * Arithmetic ADD operation on an atomic object
     *
     * @param arg   Value of second operand ADD operation
     * @return      Old value stored into atomic object
     */
    value_type fetch_add(value_type arg)
    {
        return nrf_atomic_u32_fetch_add(&m_value, arg);
    }

    /**
     * Arithmetic SUB operation on an atomic object
     *
     * @param arg   Value of second operand SUB operation
     * @return      Old value stored into atomic object
     */
    value_type fetch_sub(value_type arg)
    {
        return nrf_atomic_u32_fetch_sub(&m_value, arg);
    }

    /**
     * Logical AND operation on an atomic object
     *
     * @param arg   Value of second operand AND operation
     * @return      Old value stored into atomic object
     */
    value_type fetch_and(value_type arg)
    {
        return nrf_atomic_u32_fetch_and(&m_value, arg);
    }

    /**
     * Logical OR operation on an atomic object
     *
     * @param arg   Value of second operand OR operation
     * @return      Old value stored into atomic object
     */
    value_type fetch_or(value_type arg)
    {
        return nrf_atomic_u32_fetch_or(&m_value, arg);
    }

    /**
     * Logical XOR operation on an atomic object
     * 
     * @param arg   Value of second operand XOR operation
     * @return      Old value stored into atomic object
     */
    value_type fetch_xor(value_type arg)
    {
        return nrf_atomic_u32_fetch_xor(&m_value, arg);
    }

public:

    value_type operator++() 
    {
        return nrf_atomic_u32_add(&m_value, 1);
    }

    value_type operator++(int) 
    {
        return fetch_add(1);
    }

    value_type operator--() 
    {
        return nrf_atomic_u32_sub(&m_value, 1);
    }

    value_type operator--(int) 
    {
        return fetch_sub(1);
    }

private:

    nrf_atomic_u32_t m_value;
};

}   // namespace nrf
