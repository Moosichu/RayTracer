#include <cstddef>
#include <cmath>
#include <stdio.h>
#include <stdarg.h>

#include "color.hpp"

struct OffscreenBuffer {
    BITMAPINFO info;
    void *memory;
    int width;
    int height;
    int pitch;
    int bytesPerPixel;
};

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

struct Ray {
    Vector3D direction;
    Vector3D origin;
};

struct LightCollision {
    Vector3D position;
    Vector3D normal;
    Color ambientFactor;
    Color diffuseFactor;
    Color specularFactor;
};

struct PointLight {
    Vector3D position;
    Color color;
};

struct Sphere {
    Vector3D position;
    Color ambientFactor;
    Color diffuseFactor; //need to find a way of storing spread factor, maybe in alpha?
    Color specularFactor;
    scalar radius;

    void calculateCollision(Ray ray, LightCollision *closestCollision) {      
        Vector3D sphereToRayOrigin = ray.origin - position;
        scalar a = ray.direction.dot(ray.direction); //a = |ray.direction|^2
        scalar b = 2 * ray.direction.dot(sphereToRayOrigin);
        scalar c = sphereToRayOrigin.dot(sphereToRayOrigin) - (radius*radius);
        scalar d;
        {
            double intermediate = (b*b) - (4*a*c);
            if(intermediate < 0) {
                return; //No intersection
            }
            d = sqrt(intermediate);
        }

        double s1 = (-b + d)/(2*a);
        double s2 = (-b - d)/(2*a);
        double s = s1 < s2 ? s1 : s2; //select the closest point intersection
        if(s < 0) { //ignore collisions in negative ray direction!
            return;
        }
            
        //Work out the position of the collision relative to the camera's location
        Vector3D collisionOffset = ray.direction * s;
        if(collisionOffset.square() < closestCollision->position.square()) { //Comparing magnitudes
            closestCollision->position = collisionOffset;
            closestCollision->normal = (ray.origin - (position - collisionOffset)).normalise();
            closestCollision->ambientFactor = ambientFactor;
            closestCollision->diffuseFactor = diffuseFactor;
            closestCollision->specularFactor = specularFactor;
        }
    }
};

struct Plane {
    Vector3D position;
    Color ambientFactor;
    Color diffuseFactor;
    Color specularFactor;

    void calculateCollision(Ray ray, LightCollision *closestCollision) {
        Vector3D pDashed = position - ray.origin;
        scalar scalingFactor = pDashed.dot(pDashed)/ray.direction.dot(pDashed);
        if(scalingFactor < 0) {
            return;
        }
        Vector3D collisionOffset = ray.direction * scalingFactor;
        if(collisionOffset.square() < closestCollision->position.square()) { //Comparing magnitudes
            closestCollision->position = collisionOffset;
            closestCollision->normal  = (position * -1).normalise();
            closestCollision->ambientFactor = ambientFactor;
            closestCollision->diffuseFactor = diffuseFactor;
            closestCollision->specularFactor = specularFactor;
        }
        return;
    }
};

struct Camera {
    Vector3D position;
};

struct Screen {
    int width;
    int height;
    scalar pixelWidth;
    scalar pixelHeight;
    Vector3D position;
    Vector3D yDirection;
    Vector3D xDirection;
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

void debugPrint(const char* szFormat, ...) {
    char szBuff[1024];
    va_list arg;
    va_start(arg, szFormat);
    _vsnprintf(szBuff, sizeof(szBuff), szFormat, arg);
    va_end(arg);

    OutputDebugString(szBuff);

}

//TODO(Tom) Have ray return some other non-integer logarithmic light unit that is better
//suited for these simulations.
Color traceRay(Ray ray,
               Sphere spheres[], std::size_t numSpheres,
               Plane planes[], std::size_t numPlanes,
               PointLight lights[], std::size_t numLights,
               int recurseDepth,
               LightCollision *const defaultClosestCollision = nullptr) {

    LightCollision closestCollision;
    if(defaultClosestCollision == nullptr) {
        //This position is relative to the Ray origin. TODO(Tom): change its name to reflect that and ask
        closestCollision.position = {SCALAR_MAX, SCALAR_MAX, SCALAR_MAX}; //TODO(Tom) Replace with max possible scalar values
        //don't care about normal in this case as not used with no light factors
        //TODO(Tom): add some safety
        closestCollision.ambientFactor = {0, 0, 0};
        closestCollision.diffuseFactor = {0, 0, 0};
        closestCollision.specularFactor = {0, 0, 0};
    } else {
        closestCollision = *defaultClosestCollision;
    }

    //debugPrint("X : %f, Y : %f, Z : % f \n", ray.direction.x, ray.direction.y, ray.direction.z);
    
    //Find the closest position

    //1.Check the spheres
    for(std::size_t i = 0; i < numSpheres; i++) { //TODO(Tom) working out why numobjects was 48
        //A variable to store where the ray hit the surfact of the sphere and details about it
        spheres[i].calculateCollision(ray, &closestCollision);
    }

    //2.Check the planes
    for(std::size_t i = 0; i < numPlanes; i++) {
        planes[i].calculateCollision(ray, &closestCollision);
    }

    
    Color finalColor = closestCollision.ambientFactor;
    if(recurseDepth > 0) {
        Color diffuseComponent;
        Color specularComponent;

        //check to make sure any diffuse components actually have to be calculated
        if (closestCollision.diffuseFactor.red |
            closestCollision.diffuseFactor.green |
            closestCollision.diffuseFactor.blue) {
            for(int i = 0; i < numLights; i++) {
                PointLight light = lights[i];
                Ray newRay;
                newRay.origin = closestCollision.position + ray.origin;
                newRay.direction = light.position - newRay.origin;
                LightCollision lightCollision;
                lightCollision.position = light.position;
                //Don't care about normal as lights don't reflect light
                lightCollision.ambientFactor = light.color;
                lightCollision.diffuseFactor = {0, 0, 0};
                lightCollision.specularFactor = {0, 0, 0};


                Vector3D normalRay = newRay.direction.normalise();
                scalar diffusePower = closestCollision.normal.dot(normalRay);
                if(diffusePower < 0) break; //throw away rays which go inside the object

                Color diffuseResult = traceRay(
                    newRay,
                    spheres, numSpheres,
                    planes, numPlanes,
                    lights, numLights,
                    0,
                    &lightCollision);


                Color totalDiffuse = diffuseResult.mask(closestCollision.diffuseFactor);
                totalDiffuse.red = (uint8) (diffusePower * totalDiffuse.red);
                totalDiffuse.green = (uint8) (diffusePower * totalDiffuse.green);
                totalDiffuse.blue = (uint8) (diffusePower * totalDiffuse.blue);
                finalColor = finalColor.add(totalDiffuse);
            }
            
        }

        //check to make sure any specular components actually have to be calculated
        if (closestCollision.specularFactor.red |
            closestCollision.specularFactor.green |
            closestCollision.specularFactor.blue) {
            Ray newRay;
            newRay.origin = closestCollision.position + ray.origin;
            newRay.direction = (
                (closestCollision.normal * 2 * closestCollision.normal.dot(ray.direction)
            ) + ray.direction) * -1;
            Color specularResult = traceRay(
                newRay,
                spheres, numSpheres,
                planes, numPlanes,
                lights, numLights,
                recurseDepth - 1);
            finalColor = finalColor.add(specularResult.mask(closestCollision.specularFactor));
        } 
    }
        
    return finalColor;
}

void rayTracerMain(OffscreenBuffer backBuffer) {
    //TODO(Tom): Allocate all this stuff onto the heap!
    Camera camera;
    camera.position = {0, 0, 0};

    scalar screenAbsoluteHeight = 180.0;
    scalar screenAbsoluteWidth = 320.0;
    
    Screen screen;
    screen.width = backBuffer.width;
    screen.height = backBuffer.height;
    screen.pixelWidth = screenAbsoluteWidth / screen.width;
    screen.pixelHeight = screenAbsoluteHeight / screen.height;
    screen.position = {200.0, 0, 0};
    screen.yDirection = {0, 1, 0}; //TODO(Tom) Encode Pixel width and height in here in a neat way
    screen.xDirection = {0, 0, 1};

    Sphere spheres[3];
    
    Sphere sphere0;
    sphere0.position = {300.0, 0, 0};
    sphere0.radius = 50.0;
    sphere0.ambientFactor = {0,0,0};
    sphere0.diffuseFactor = {50, 50, 50};
    sphere0.specularFactor = {200, 200, 200};
    spheres[0] = sphere0;

    Sphere sphere1;
    sphere1.position = {400.0, 50.0, 20.0};
    sphere1.radius = 40.0;
    sphere1.ambientFactor = {0, 0, 0};
    sphere1.diffuseFactor = {50, 50, 50};
    sphere1.specularFactor = {100, 100, 100};
    spheres[1] = sphere1;

    Sphere sphere2;
    sphere2.position = {220.0, -50.0, -20.0};
    sphere2.radius = 30.0;
    sphere2.ambientFactor = {10, 50, 100};
    sphere2.diffuseFactor = {50, 50, 50};
    sphere2.specularFactor = {100, 100, 130};
    spheres[2] = sphere2;

    //TODO(Tom): add planes here.
    Plane planes[1];

    Plane plane0;
    plane0.position = {0, -100, 0};
    plane0.ambientFactor = {10, 10, 10};
    plane0.diffuseFactor = {100, 100, 100}; //TODO(Tom) This isn't working :( Plane diffuse not appearing
    plane0.specularFactor = {100, 100, 100};
    planes[0] = plane0;
    
    PointLight lights[2];
    PointLight light0;
    light0.position = {0.0, 255.0, 255.0};
    light0.color = {255, 255, 255};
    lights[0] = light0;

    PointLight light1;
    light1.position = {400, 20, -50};
    light1.color = {255, 255, 255};
    lights[1] = light1;

    for(std::size_t x = 0; x < screen.width; x++) {
        for(std::size_t y = 0; y < screen.height; y++) {
            Ray ray;
            //This block calculates the direction from the camera to
            //the position of the virtual pixel in the virtual screen
            {
                //calculates the pixel's offset from the centre of the
                //screen in the virtual space
                Vector3D pixelOffset =
                    (screen.yDirection * (((double) screen.height/2) - y) * screen.pixelHeight) +
                    (screen.xDirection * (x - ((double) screen.width/2)) * screen.pixelWidth);

                //calculate the screen's pixel location in the world!
                //TODO(Tom) take the screen normal into account!
                Vector3D pixelLocation = screen.position + pixelOffset;
                
                //calculate the ray now!
                ray.direction = pixelLocation - camera.position;
                ray.origin = camera.position;
            }
             Color pixelColor = traceRay(ray,
                                      spheres,
                                      sizeof(spheres)/sizeof(spheres[0]),
                                      planes,
                                      sizeof(planes)/sizeof(planes[0]),
                                      lights,
                                      sizeof(lights)/sizeof(lights[0]),
                                      1);
            setPixel(backBuffer, x, y, pixelColor);
        }
    }
}
