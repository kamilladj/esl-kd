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
        buff.push_back((val >> 8) & 0xFF);
        buff.push_back(val & 0xFF);
    }

    template<size_t size>
    void serialize(static_vector<uint8_t, size>& buff, int16_t val)
    {
        buff.push_back((val >> 8) & 0xFF);
        buff.push_back(val & 0xFF);
    }

    //template<size_t size>
    //void serialize(static_vector<uint8_t, size>& buff, uint32_t val)
    //{}

    //template<size_t size>
    //void serialize(static_vector<uint8_t, size>& buff, int32_t val)
    //{}

    //template<size_t size>
    //void serialize(static_vector<uint8_t, size>& buff, uint64_t val)
    //{}

    //template<size_t size>
    //void serialize(static_vector<uint8_t, size>& buff, int64_t val)
    //{}

    //template<size_t size>
    //void serialize(static_vector<uint8_t, size>& buff, char val)
    //{}

    //template<size_t size>
    //void serialize(static_vector<uint8_t, size>& buff, short int val)
    //{}

    //template<size_t size>
    //void serialize(static_vector<uint8_t, size>& buff, int val)
    //{}

    //template<size_t size>
    //void serialize(static_vector<uint8_t, size>& buff, long int val)
    //{}

    //template<size_t size>
    //void serialize(static_vector<uint8_t, size>& buff, long long int val)
    //{}

    //template<size_t size>
    //void serialize(static_vector<uint8_t, size>& buff, double val)
    //{}

    //template<size_t size>
    //void serialize(static_vector<uint8_t, size>& buff, long double val)
    //{}

    //template<size_t size>
    //void serialize(static_vector<uint8_t, size>& buff, float vall)
    //{}
}