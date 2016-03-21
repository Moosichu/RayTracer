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
    *((Uint32*)pixel) = SDL_MapRGBA(
        buffer.screen->format,
        color.red, color.green, color.blue, color.alpha
    );
}

int main() {
    printf("hello, world!\n");

    OffscreenBuffer globalBackbuffer;
    globalBackbuffer.width = 1052;
    globalBackbuffer.height = 526;
    globalBackbuffer.bytesPerPixel = 32;

    printf("Starting\n");
    //The window that is being rendered to
    SDL_Window* window = NULL;

    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, globalBackbuffer.width, globalBackbuffer.height, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            screenSurface = SDL_GetWindowSurface(window);
            globalBackbuffer.screen = screenSurface;

            //SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0xFF));
            rayTracerInitialise(globalBackbuffer);
            rayTracerMain(globalBackbuffer);

            SDL_UpdateWindowSurface(window);

            SDL_Delay(7000);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Finished\n");
    return 0;

}
