#ifndef COLOR_H
#define COLOR_H
#include "header.h"

struct Color {
    static const Color BLACK;
    static const Color WHITE;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    
    
    uint8 red;
    uint8 green;
    uint8 blue;
    uint8 alpha;
    
    Color add(Color c);

    Color mask(Color c);

private:
    static uint8 addChannels(uint8 c1, uint8 c2);
};
#endif
