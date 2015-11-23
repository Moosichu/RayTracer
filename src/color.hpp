#ifndef COLOR_H
#define COLOR_H
#include "header.h"

struct Color {
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
