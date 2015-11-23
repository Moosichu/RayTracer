#include <cstddef>

#include "header.h"
#include "os_layer.h"

void lineDrawerMain(OffscreenBuffer backBuffer) {
    //TODO(Tom): draw lines to backbuffer

    for(std::size_t x = 0; x < backBuffer.width; x++) {
        for(std::size_t y = 0; y < backBuffer.height; y++) {
            Color color = {
                (uint8) x,
                (uint8) y,
                (uint8) (x + y)};
            setPixel(backBuffer, x, y, color);
        }
    }
}
