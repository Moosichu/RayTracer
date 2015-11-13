
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
    uint8 alpha;
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
};


struct Screen {
    int width;
    int height;
    scalar pixelSize;
    Vector3D position;
    Vector3D normal;
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
};

struct Sphere {
    Vector3D position;
    Color ambientFactor;
    Color diffuseFactor; //need to find a way of storing spread factor, maybe in alpha?
    Color specularFactor;
    scalar radius;
};

struct Camera {
    Vector3D position;
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

Color traceRay(Ray ray, Sphere sceneObjects[], PointLight lights[], int recurseDepth) {
    LightCollision closestCollision;
    closestCollision.position = {600000.0, 60000.0, 60000.0};
    closestCollision.normal = {0, 0, 0};
    closestCollision.ambientFactor = {0, 0, 0};
    closestCollision.diffuseFactor = {0, 0, 0};
    closestCollision.specularFactor = {0, 0, 0};
    return {0, 0, 0};
}

void rayTracerMain(OffscreenBuffer backBuffer) {
    Camera camera;
    camera.position = {0, 0, 0};

    Screen screen;
    screen.width = backBuffer.width;
    screen.height = backBuffer.height;
    screen.pixelSize = 1.0;
    screen.position = {0, 0, 10.0};
    screen.normal = {0,0,-1.0};

    Sphere sceneObjects[1];
    Sphere sphere0;
    sphere0.position = {0, 20, 70};
    sphere0.ambientFactor = {255, 255, 255};

    sceneObjects[0] = sphere0;

    PointLight lights[1];

    for(int x = 0; x < screen.width; x++) {
        for(int y = 0; y < screen.height; y++) {
            Ray ray;
            //This block calculates the direction from the camera to
            //the position of the virtual pixel in the virtual screen
            {
                //calculates the pixel's offset from the centre of the
                //screen in the virtual space
                Vector3D pixelOffset = {
                    (x - ((double) screen.width/2)) * screen.pixelSize,
                    (y - ((double) screen.height/2)) * screen.pixelSize,
                    0
                };

                //calculate the screen's pixel location in the world!
                //TODO(Tom) take the screen normal into account!
                Vector3D pixelLocation = screen.position + pixelOffset;
                
                //calculate the ray now!
                ray.direction = pixelLocation - camera.position;
                ray.origin = camera.position;
            }
            Color pixelColor = traceRay(ray, sceneObjects, lights, 0);
            setPixel(backBuffer, x, y, pixelColor);
        }
    }
}
