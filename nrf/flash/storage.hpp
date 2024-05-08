#pragma once

#include "hsv.hpp"
#include "memory.hpp"
#include "serializer.hpp"

#include "static_vector.hpp"
#include "crc16.hpp"

#include <stdint.h>
#include <stddef.h>
#include <cstring>

namespace nrf
{
    template<typename T, size_t record_size = 16>
    class storage
    {
    private:

        bool is_record_header_valid(const static_vector<uint8_t, record_size>& buff)
        {
            uint16_t crc = (buff[5] << 8) | buff[6];

            return crc == utils::crc16(&buff[0], 5);
        }

        void fill_record_header(static_vector<uint8_t, record_size>& buff, const T& obj)
        {
            buff.push_back(0);
            buff.push_back(92);
            buff.push_back(10); //id 23562
            buff.push_back(0);
            buff.push_back(125); //version 125
            buff.push_back(67);
            buff.push_back(254); //crc 17406
            buff.push_back(0);
            buff.push_back(0); //reserved
            buff.push_back(sizeof(obj)); //body size
        }

    private:

        void serialize(static_vector<uint8_t, record_size>& buff, const T& obj)
        {
            nrf::serialize<record_size>(buff, obj.get_hue());
            nrf::serialize<record_size>(buff, obj.get_sat());
            nrf::serialize<record_size>(buff, obj.get_val());
        }

        void deserialize(T& obj, const static_vector<uint8_t, record_size>& buff)
        {
            uint16_t h = (buff[10] << 8) | buff[11];
            uint16_t s = (buff[12] << 8) | buff[13];
            uint16_t v = (buff[14] << 8) | buff[15];
            obj = hsv(h, s, v);
        }

    public:

        void save(const T& obj)
        {
            static_vector<uint8_t, record_size> buff;
            
            fill_record_header(buff, obj);
            
            serialize(buff, obj);

            m_memory.write_record_to_page(buff);
        }

        bool load(T& obj)
        {
            static_vector<uint8_t, record_size> buff;

            bool res = m_memory.read_record_from_page(buff);

            if (res && is_record_header_valid(buff))
                deserialize(obj, buff);

            return res;
        }

    private:

        memory<record_size> m_memory;
    };
}