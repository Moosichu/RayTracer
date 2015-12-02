#include <cstddef>

#include "header.h"
#include "os_layer.h"

#include "vector2d.hpp"

struct Line {
    Vector2D point_0;
    Vector2D point_1;
};


//Draw a line
//TODO(Tom): Review it from scratch
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
    while((x*xdir) <= (round(x1)*xdir)) {
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
    line1.point_0 = {250.0, 250.0};
    line1.point_1 = {500.0, 250.0};

    //TODO(Tom) Handle vertical lines
    Line line2;
    line2.point_0 = {250.0, 250.0};
    line2.point_1 = {500.0, 500.0};
    
    Line line3;
    line3.point_0 = {250.0, 250.0};
    line3.point_1 = {250.0, 500.0};

    Line line4;
    line4.point_0 = {250.0, 250.0};
    line4.point_1 = {0.0, 500.0};

    Line line5;
    line5.point_0 = {250.0, 250.0};
    line5.point_1 = {0.0, 250.0};

    Line line6;
    line6.point_0 = {250.0, 250.0};
    line6.point_1 = {0.0, 0.0};

    Line line7;
    line7.point_0 = {250.0, 250.0};
    line7.point_1 = {250.0, 0.0};


    Line line8;
    line8.point_0 = {250.0, 250.0};
    line8.point_1 = {500.0, 0.0};

    //Clear the buffer to white
    for(std::size_t x = 0; x < backBuffer.width; x++) {
        for(std::size_t y = 0; y < backBuffer.height; y++) {
            Color color = {255, 255, 255, 255};
            setPixel(backBuffer, x, y, color);
        }
    }
    drawLine(line1, backBuffer);
    drawLine(line2, backBuffer);
    drawLine(line3, backBuffer);
    drawLine(line4, backBuffer);
    drawLine(line5, backBuffer);
    drawLine(line6, backBuffer);
    drawLine(line7, backBuffer);
    drawLine(line8, backBuffer);
}

