#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "os_layer.h"

///
/// Initialises everything needed for the raytracer
///  (Allocates memory, etc)
///
void rayTracerInitialise(OffscreenBuffer backBuffer);

///
/// Draw a frame of the desired ray-traced scene to the bacBuffer
///
void rayTracerMain(OffscreenBuffer backBuffer);

#endif
