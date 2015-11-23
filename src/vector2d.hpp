#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "header.h"

struct Vector2D {
    scalar x;
    scalar y;

    Vector2D operator+(const Vector2D v) {
        return {x + v.x, y + v.y};
    }

    Vector2D operator-(const Vector2D v) {
        return {x - v.x, y - v.y};
    }

    Vector2D operator*(const scalar factor) {
        return {factor * x, factor * y};
    }

    Vector2D normalise() {
        scalar mag = magnitude();
        return {x/mag, y/mag};
    }

    scalar dot(const Vector2D v) {
        return (x * v.x) + (y * v.y);
    }

    scalar square() {
        return dot(*this);
    }

    scalar magnitude() {
        return sqrt(square());
    }
};

#endif
