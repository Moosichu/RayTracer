#include "header.h"
#include <cstdarg>
#include <stdio.h>

#include "os_layer.h"
#include "ray_tracer.hpp"
#include "line_drawer.hpp"

void debugPrint(const char* szFormat, ...) {
    va_list args;
    va_start(args, szFormat);
    vprintf(szFormat, args);
    va_end(args);
}

void setPixel(OffscreenBuffer buffer, int x, int y, Color color) {
    Uint8 * pixel = (Uint8*)buffer.screen->pixels;
    pixel += (y * buffer.screen->pitch) + (x * sizeof(Uint32));
    *((Uint32*)pixel) = SDL_MapRGBA(buffer.screen->format, color.red, color.green, color.blue, color.alpha);
}

int main() {
    printf("hello, world!\n");
    
    OffscreenBuffer globalBackbuffer;
    globalBackbuffer.width = 1052;
    globalBackbuffer.height = 526;
    globalBackbuffer.bytesPerPixel = 32;

    SDL_Init(SDL_INIT_VIDEO);
    globalBackbuffer.screen = SDL_SetVideoMode(globalBackbuffer.width , globalBackbuffer.height, globalBackbuffer.bytesPerPixel, SDL_SWSURFACE);

    #ifdef TEST_SDL_LOCK_OPTS
    EM_ASM("SDL.defaults.copyOnLock = false; SDL.defaults.discardOnLock = true; SDL.defaults.opaqueFrontBuffer = false;");
    #endif

    if (SDL_MUSTLOCK(globalBackbuffer.screen)) SDL_LockSurface(globalBackbuffer.screen);
    //for(;;) {
        //rayTracerMain(globalBackbuffer);
        //lineDrawerMain(globalBackbuffer);
    //}
    for (int i = 0; i < globalBackbuffer.width; i++) {
      for (int j = 0; j < globalBackbuffer.height; j++) {
        #ifdef TEST_SDL_LOCK_OPTS
        // Alpha behaves like in the browser, so write proper opaque pixels.
        int alpha = 255;
       #else
        // To emulate native behavior with blitting to screen, alpha component is ignored. Test that it is so by outputting
        // data (and testing that it does get discarded)
        int alpha = (i+j) % 255;
        #endif
        Color color;
        color.red = i;
        color.blue = j;
        color.green = 255-i;
        color.alpha = alpha;
        setPixel(globalBackbuffer, i, j, color);
      }
    }
    if (SDL_MUSTLOCK(globalBackbuffer.screen)) SDL_UnlockSurface(globalBackbuffer.screen);
    SDL_Flip(globalBackbuffer.screen); 
    SDL_Quit();
    printf("Finish!!!!");
    return 0;

}

