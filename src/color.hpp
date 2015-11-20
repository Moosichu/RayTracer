#include <stdint.h>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

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
