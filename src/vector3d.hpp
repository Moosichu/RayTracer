#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "header.h"

//TODO(Tom) work out how to make these consts!
struct Vector3D {
    scalar x;
    scalar y;
    scalar z;

    Vector3D operator+(const Vector3D v) {
        return {x + v.x, y + v.y, z + v.z};
    }

    Vector3D operator-(const Vector3D v) {
        return {x - v.x, y - v.y, z - v.z};
    }

    Vector3D operator*(const scalar factor) {
        return {factor * x, factor * y, factor * z};
    }

    Vector3D normalise() {
        scalar mag = magnitude();
        return {x/mag, y/mag, z/mag};
    }

    scalar dot(const Vector3D v) {
        return (x * v.x) + (y * v.y) + (z * v.z);
    }

    scalar square() {
        return dot(*this);
    }

    scalar magnitude() {
        return sqrt(square());
    }
};

#endif
