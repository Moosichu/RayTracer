#ifndef OS_LAYER_H
#define OS_LAYER_H

#include "color.hpp"

//TODO(Tom) REMOVE THIS DEPENDENCY FOR THIS DATA STRUCTURE!!!!!
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>

struct OffscreenBuffer {
    BITMAPINFO info;
    void *memory;
    int width;
    int height;
    int pitch;
    int bytesPerPixel;
};

#elif defined(__EMSCRIPTEN__)

#include <SDL/SDL.h>
#include <emscripten.h>

struct OffscreenBuffer {
    int width;
    int height;
    int pitch;
    int bytesPerPixel;
    SDL_Surface *screen; 
};


#else  //TODO: work out emscripten macro

struct OffscreenBuffer {
    int width;
    int height;
    int pitch;
    int bytesPerPixel;
};

#endif


/*
 * Set the pixel in the buffer to the given colour!
 */
void setPixel(OffscreenBuffer buffer, int x, int y, Color color);

void debugPrint(const char* szFormat, ...); //TODO: create a wrappable version!!!

#endif
