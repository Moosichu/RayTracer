#include "color.hpp"

const Color Color::BLACK = {0, 0, 0, 255};
const Color Color::WHITE = {255, 255, 255, 255};
const Color Color::RED = {255, 0, 0, 255};
const Color Color::GREEN = {0, 255, 0, 255};
const Color Color::BLUE = {0, 0, 255, 255};

Color Color::add(Color c) {
    Color r;
    r.red = addChannels(red, c.red);
    r.green = addChannels(green, c.green);
    r.blue = addChannels(blue, c.blue);
    return r;
}

Color Color::mask(Color c) {
    Color r;
    r.red = (uint8) (((double) red * c.red)/255);
    r.green = (uint8) (((double) green * c.green)/255);
    r.blue = (uint8) (((double) blue * c.blue)/255);
    return r;
}


uint8 Color::addChannels(uint8 c1, uint8 c2) {
    if((c1 | c2) == 255) {
        c1 = 255;
    } else {
        uint8 newC = c1 + c2;
        if(newC < (c1 | c2)) {
            //Have overflow, max out color!
            c1 = 255;
        } else {
            c1 = newC;
        }
    }
    return c1;
}

