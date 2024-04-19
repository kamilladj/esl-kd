#pragma once

#include <stdint.h>
#include <stddef.h>

#include "static_vector.hpp"

namespace nrf
{
    template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, uint8_t val)
    {
        buff.push_back(val & 0xFF);
    }

    template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, int8_t val)
    {
        buff.push_back(val & 0xFF);
    }

    template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, uint16_t val)
    {
        buff.push_back(val & 0xFF);
        buff.push_back((val >> 8) & 0xFF);
    }

    template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, int16_t val)
    {
        buff.push_back(val & 0xFF);
        buff.push_back((val >> 8) & 0xFF);
    }

    template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, uint32_t val)
    {
        buff.push_back(val & 0xFF);
        buff.push_back((val >> 8) & 0xFF);
        buff.push_back((val >> 16) & 0xFF);
        buff.push_back((val >> 24) & 0xFF);
    }

    template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, int32_t val)
    {
        buff.push_back(val & 0xFF);
        buff.push_back((val >> 8) & 0xFF);
        buff.push_back((val >> 16) & 0xFF);
        buff.push_back((val >> 24) & 0xFF);
    }

    template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, uint64_t val)
    {
        buff.push_back(val & 0xFF);
        buff.push_back((val >> 8) & 0xFF);
        buff.push_back((val >> 16) & 0xFF);
        buff.push_back((val >> 24) & 0xFF);
        buff.push_back((val >> 32) & 0xFF);
        buff.push_back((val >> 40) & 0xFF);
        buff.push_back((val >> 48) & 0xFF);
        buff.push_back((val >> 56) & 0xFF);
    }

    template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, int64_t val)
    {
        buff.push_back(val & 0xFF);
        buff.push_back((val >> 8) & 0xFF);
        buff.push_back((val >> 16) & 0xFF);
        buff.push_back((val >> 24) & 0xFF);
        buff.push_back((val >> 32) & 0xFF);
        buff.push_back((val >> 40) & 0xFF);
        buff.push_back((val >> 48) & 0xFF);
        buff.push_back((val >> 56) & 0xFF);
    }

    template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, char val)
    {
        buff.push_back(val & 0xFF);
    }

    /*template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, short int val)
    {
        size_t res = 0;

        for (size_t i = 0; i < sizeof(long long int); i++, res += 8)
            buff.push_back((val >> res) & 0xFF);
    }*/

    template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, int val)
    {
        size_t res = 0;

        for (size_t i = 0; i < sizeof(long long int); i++, res += 8)
            buff.push_back((val >> res) & 0xFF);
    }

    /*template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, long int val)
    {
        size_t res = 0;

        for (size_t i = 0; i < sizeof(long long int); i++, res += 8)
            buff.push_back((val >> res) & 0xFF);
    }

    template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, long long int val)
    {
        size_t res = 0;

        for (size_t i = 0; i < sizeof(long long int); i++, res += 8)
            buff.push_back((val >> res) & 0xFF);
    }*/
}