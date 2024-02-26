#pragma once

#include "hsv.hpp"
#include "memory.hpp"

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

        //void serialize(static_vector<uint8_t, size>& buff, uint8_t val)
        //{
        //    buff[0] = val & 0xFF;
        //}

        //void serialize(static_vector<int8_t, size>& buff, int8_t val)
        //{
        //    buff[0] = val & 0xFF;
        //}

        //void serialize(static_vector<uint8_t, size>& buff, uint16_t val)
        //{}

        //void serialize(static_vector<uint8_t, size>& buff, int16_t val)
        //{}

        //void serialize(static_vector<uint8_t, size>& buff, uint32_t val)
        //{}

        //void serialize(static_vector<uint8_t, size>& buff, int32_t val)
        //{}

        //void serialize(static_vector<uint8_t, size>& buff, uint64_t val)
        //{}

        //void serialize(static_vector<uint8_t, size>& buff, int64_t val)
        //{}

        //void serialize(static_vector<uint8_t, size>& buff, char val)
        //{}

        //void serialize(static_vector<uint8_t, size>& buff, short int val)
        //{}

        //void serialize(static_vector<uint8_t, size>& buff, int val)
        //{}

        //void serialize(static_vector<uint8_t, size>& buff, long int val)
        //{}

        ////void serialize(static_vector<uint8_t, size>& buff, long long int val)
        ////{}

        //void serialize(static_vector<uint8_t, size>& buff, double val)
        //{}

        //void serialize(static_vector<uint8_t, size>& buff, long double val)
        //{}

        //void serialize(static_vector<uint8_t, size>& buff, float vall)
        //{}

        void serialize(const T& obj, static_vector<uint8_t, size>& buff)
        {
            memcpy(&buff[size - sizeof(obj)], &obj, sizeof(obj));
        }

        void deserialize(const static_vector<uint8_t, size>& buff, T& obj)
        {
            memcpy(&obj, &buff[size - sizeof(obj)], sizeof(obj));
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
            
            serialize(obj, buff);

            m_memory.write_to_page(buff);
        }

        bool load(T& obj) //be bool
        {
            static_vector<uint8_t, size> buff;

            m_memory.read_from_page(buff);

            deserialize(buff, obj);

            return true;
        }

    private:

        memory<size> m_memory;
    };
}