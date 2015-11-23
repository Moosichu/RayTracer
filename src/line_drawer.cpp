#include <cstddef>

#include "header.h"
#include "os_layer.h"

#include "vector2d.hpp"

struct Line {
    Vector2D point_0;
    Vector2D point_1;
};

static void drawLine(Line line, OffscreenBuffer backBuffer) {
    scalar x0, y0, x1, y1;
    x0 = line.point_0.x;
    y0 = line.point_0.y;
    x1 = line.point_1.x;
    y1 = line.point_1.y;
        
    scalar xdiff = x1 - x0;
    scalar ydiff = y1 - y0;

    int xdir = 1;
    if(xdiff < 0) {
        xdir = -1;
    }

    int ydir = 1;
    if(ydiff < 0) {
        ydir = -1;
    }
    
    scalar m  = (ydiff)/(xdiff);
    int64 x = round(x0);
    scalar yi = y0 + m * (x - x0);
    int64 y = round(yi);
    scalar yf = yi - y;
    while(x <= round(x1)) {
        setPixel(backBuffer, x, y, Color::BLACK);
        x = x + xdir;
        yf = yf + m;
        if(yf > 1/2) {
            y = y + ydir;
            yf = yf - ydir;
        }
    }
}

void lineDrawerMain(OffscreenBuffer backBuffer) {
    Line line1;
    line1.point_0 = {0.0, 0.0};
    line1.point_1 = {100000.0, 1000000.0};

    Line line2;
    line2.point_0 = {0.0, 250.0};
    line2.point_1 = {250.0, 250.0};
    
    for(std::size_t x = 0; x < backBuffer.width; x++) {
        for(std::size_t y = 0; y < backBuffer.height; y++) {
            //Color color = {
            //    (uint8) x,
            //    (uint8) y,
            //   (uint8) (x + y)};
            Color color = {255, 255, 255, 255};
            setPixel(backBuffer, x, y, color);
        }
    }
    drawLine(line1, backBuffer);
    drawLine(line2, backBuffer);
}

