#pragma once

#include "hsv.hpp"
#include "memory.hpp"
#include "serializer.hpp"

#include "static_vector.hpp"

#include <stdint.h>
#include <stddef.h>
#include <cstring>

namespace nrf
{
    template<typename T, size_t size>
    class storage
    {
    public:

        storage()
        {}

    private:

        void serialize(static_vector<uint8_t, size>& buff, const T& obj)
        {
            nrf::serialize<size>(buff, obj.get_hue());
            nrf::serialize<size>(buff, obj.get_sat());
            nrf::serialize<size>(buff, obj.get_val());
        }

        void deserialize(T& obj, const static_vector<uint8_t, size>& buff)
        {
            uint16_t h = ((buff[11] << 8) | buff[10]);
            uint16_t s = ((buff[13] << 8) | buff[12]);
            uint16_t v = ((buff[15] << 8) | buff[14]);
            obj = hsv(h, s, v);
        }

    public:

        void save(const T& obj)
        {
            static_vector<uint8_t, size> buff;
            buff.push_back(0);
            buff.push_back(92);
            buff.push_back(10); //23562
            buff.push_back(0);
            buff.push_back(125); //version 125
            buff.push_back(0);
            buff.push_back(7); //crc 111
            buff.push_back(0);
            buff.push_back(0); //reserved
            buff.push_back(sizeof(obj)); //body size
            
            serialize(buff, obj);

            m_memory.write_to_page(buff);
        }

        bool load(T& obj)
        {
            static_vector<uint8_t, size> buff;

            bool res = m_memory.read_from_page(buff);

            deserialize(obj, buff);

            return res;
        }

    private:

        memory<size> m_memory;
    };
}