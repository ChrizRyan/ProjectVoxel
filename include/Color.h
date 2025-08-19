#pragma once

#include <cstdint>

struct Color
{
    static const int MAX = 0xffff;

    uint8_t r, g, b, a;
    Color(uint8_t r=0, uint8_t g=0, uint8_t b=0, uint8_t a=MAX) : r(r), g(g), b(b), a(a) {}   
};

namespace Colors
{
    const Color Red = Color(1,0,0,1);
    const Color Green = Color(0,1,0,1);
    const Color Blue = Color(0,0,1,1); 
    const Color Yellow = Color(1,1,0,1);
    const Color Cyan = Color(0,1,1,1);
    const Color Magenta = Color(1,0,1,1);
    const Color White = Color(1,1,1,1);
    const Color Black = Color(0,0,0,1);
    const Color Gray = Color(0.5f, 0.5f, 0.5f, 1.0f);
    const Color Transparent = Color(0, 0, 0, 0);
    const Color SemiTransparentGrey = Color(0.5f, 0.5f, 0.5f, 0.5f);
};

