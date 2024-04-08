#pragma once

#include "hsv.hpp"
#include "memory.hpp"
//#include "serializer.hpp"

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
            memcpy(&buff[size - sizeof(obj)], &obj, sizeof(obj));

            //serialize<size>(buff, obj.get_hue());
            //serialize<size>(buff, obj.get_sat());
            //serialize<size>(buff, obj.get_val());
        }

        void deserialize(T& obj, const static_vector<uint8_t, size>& buff)
        {
            memcpy(&obj, &buff[size - sizeof(obj)], sizeof(obj));

            /*uint16_t h = (buff[10] << 8) | buff[11]);
            uint16_t s = (buff[12] << 8) | buff[13]);
            uint16_t v = (buff[14] << 8) | buff[15]);
            obj = hsv(h, s, v);*/
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

            m_memory.read_from_page(buff);

            deserialize(obj, buff);

            return true;
        }

    private:

        memory<size> m_memory;
    };
}