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
  //TODO: set the pixel in an html5 canvas!
}

int main() {
  printf("hello, world!\n");
  //Create HTML canvas with OffscreenBuffer as pointer to it
  OffscreenBuffer globalBackbuffer;
  for(;;) {
    rayTracerMain(globalBackbuffer);
  }
  return 0;
}

