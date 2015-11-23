#ifndef OS_LAYER_H
#define OS_LAYER_H

//TODO(Tom) REMOVE THIS DEPENDENCY FOR THIS DATA STRUCTURE!!!!!
#include <windows.h>
#include "color.hpp"

struct OffscreenBuffer {
    BITMAPINFO info;
    void *memory;
    int width;
    int height;
    int pitch;
    int bytesPerPixel;
};


/*
 * Set the pixel in the buffer to the given colour!
 */
void setPixel(OffscreenBuffer buffer, int x, int y, Color color);

void debugPrint(const char* szFormat, ...);

#endif
