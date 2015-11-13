
struct OffscreenBuffer {
    BITMAPINFO info;
    void *memory;
    int width;
    int height;
    int pitch;
    int bytesPerPixel;
};

struct Color {
    uint8 red;
    uint8 green;
    uint8 blue;
};

struct Vector3D {
    scalar x;
    scalar y;
    scalar z;
};

struct SceneObject {
    

};



/*
 * Set the pixel in the buffer to the given colour!
 */
static void setPixel(OffscreenBuffer buffer, int x, int y, Color color) {
    uint8 *row = (uint8 *) buffer.memory;
    row += buffer.pitch * y;
    uint32 *pixel = (uint32 *) row;
    pixel += x;
    *pixel = ((color.red << 16) | (color.green << 8) | color.blue);
}


internal void renderWeirdGradient2(OffscreenBuffer buffer, int xOffset, int yOffset) {
    for(int y = 0; y < buffer.height; y++) {
        for(int x = 0; x < buffer.width; x++) {
            /*
              Pixel  in  memory: BB GG RR xx

              Pixel in register: xx RR GG BB

              ^^Due to LITTLE ENDIAN architecture
             */
            Color color;
            color.blue = (x + xOffset);
            color.green = (y + yOffset);
            color.red = (color.blue + color.green)*3;
            setPixel(buffer, x, y, color);
        }
    }

}


void rayTracerMain(OffscreenBuffer backBuffer) {
    renderWeirdGradient2(backBuffer, 0, 0);
}
