#pragma once

#include "hsv.hpp"

#include <stdint.h>

namespace nrf
{
    struct rgb
    {
        rgb(uint16_t r, uint16_t g, uint16_t b)
            : red{r}
            , green{g}
            , blue{b}
        {}

        rgb(hsv hsv)
        {
            *this = create_rgb(hsv.get_hue(), hsv.get_sat(), hsv.get_val());
        }

        static rgb create_rgb(uint16_t hue, uint16_t sat, uint16_t val)
        {
            if (sat == 0)
                return rgb(val, val, val);
            else
                return create_rgb_from_region(hue, sat, val);
        }

        // from https://stackoverflow.com/a/14733008
        static rgb create_rgb_from_region(uint16_t hue, uint16_t sat, uint16_t val)
        {
            uint16_t region, remainder, p, q, t;

            region = hue / 43;
            remainder = (hue - (region * 43)) * 6;

            p = (val * (255 - sat)) >> 8;
            q = (val * (255 - ((sat * remainder) >> 8))) >> 8;
            t = (val * (255 - ((sat * (255 - remainder)) >> 8))) >> 8;

            switch (region)
            {
            case 0:
                return rgb(val, t, p);
            case 1:
                return rgb(q, val, p);
            case 2:
                return rgb(p, val, t);
            case 3:
                return rgb(p, q, val);
            case 4:
                return rgb(t, p, val);
            default:
                return rgb(val, p, q);
            }
        }

        uint16_t red;
        uint16_t green;
        uint16_t blue;
    };
}