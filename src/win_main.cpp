/**
 *  The main entry point for windows
 */

//NOTE(moosichu): Thank you so so so so so so so much Casey Muratori for creating the handmade hero videos to help me learn all of this :)
#include <windows.h>
#include <stdint.h>
#include <xinput.h>

//Make static's overloading based on usage clearer
#define internal static         //Use on functions where static means that functions are only accessable in local translation unit
#define local_persist static    //Create alternate def for static, so usage is clear in context (and this type can be removed later)
#define global_variable static  //Use on variables where static makes them global

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef double scalar;

#include "ray_tracer.cpp"

//TODO(moosichu) Maybe handle this some other way

global_variable bool gameRunning;       //Globals in platform layer isn't too bad, for now...
global_variable OffscreenBuffer globalBackbuffer;

internal void renderWeirdGradient(OffscreenBuffer buffer, int xOffset, int yOffset) {
    //TODO(moosichu) Look at what the optimiser does with buffer by value/pointer

    uint8 *row = (uint8 *) buffer.memory; //Cast it so we can move pointer by 8 bits each way
    for(int y = 0; y < buffer.height; y++) {
        uint32 *pixel = (uint32 *) row; //Cast to pixel size
        for(int x = 0; x < buffer.width; x++) {
            /*
              Pixel  in  memory: BB GG RR xx

              Pixel in register: xx RR GG BB

              ^^Due to LITTLE ENDIAN architecture
             */
            uint8 blue = (x + xOffset);
            uint8 green = (y + yOffset);
            uint8 red = (blue + green)*3;
            *pixel++ = ((red << 16) | (green << 8) | blue);
        }
        row += buffer.pitch; //Move onto the next row
    }

}

/**
 * Create the backbuffer
 */
internal OffscreenBuffer resizeDIBSection(OffscreenBuffer buffer, int width, int height) {
    if(buffer.memory) {
        VirtualFree(buffer.memory, 0, MEM_RELEASE);
    }

    buffer.width = width;
    buffer.height = height;
    buffer.bytesPerPixel = 4;
    
    buffer.info.bmiHeader.biSize = sizeof(buffer.info.bmiHeader);
    buffer.info.bmiHeader.biWidth = buffer.width;
    buffer.info.bmiHeader.biHeight = -buffer.height;  //Make the drawing top-down
    buffer.info.bmiHeader.biPlanes = 1;
    buffer.info.bmiHeader.biBitCount = 32;           //Size per bit, only use 24 but need allignment
    buffer.info.bmiHeader.biCompression = BI_RGB;    //Want no compression

    int bitmapMemorySize = (buffer.width*buffer.height)*buffer.bytesPerPixel;
    buffer.memory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE); //Allocate the memory needed
    //TODO: clear this to black

    buffer.pitch = buffer.width*buffer.bytesPerPixel;

    return buffer;
}

/**
 * Draw to the window
 */
internal void copyBufferToDisplayWindow(HDC deviceContext, RECT clientRect,
                                        OffscreenBuffer buffer,
                                        int x, int y, int width, int height) {
    int windowWidth = clientRect.right - clientRect.left;
    int windowHeight = clientRect.bottom - clientRect.top;
    
    //Copy the bits from the backbuffer to the window
    StretchDIBits(deviceContext,
                  // x, y, width, height,  //Size of window
                  //x, y, width, height,  //Size of backbuffer
                  0, 0, buffer.width, buffer.height,
                  0, 0, windowWidth, windowHeight,
                  buffer.memory,
                  &buffer.info,
                  DIB_RGB_COLORS,       //Direct RGB writing to the buffer
                  SRCCOPY);             //Copy source into destination
}



/**
 * The callback function that handles windows events.
 * (It is what is given to WNDCLASS WindowClass.lpfnWndProc.)
 *
 */
LRESULT mainWindowCallback(
    HWND windowHandle, //The handle to the window (opaque datastructure)
    UINT message,              //The message windows is asking us to handle
    WPARAM wParam,
    LPARAM lParam)
{
    LRESULT result = 0; //What we return when we process a message (0) means handled
    
    switch(message) {
        case WM_SIZE: {
            RECT clientRect; //Area you can draw to
            GetClientRect(windowHandle, &clientRect);
            int width = clientRect.right - clientRect.left;
            int height = clientRect.bottom - clientRect.top;
            globalBackbuffer = resizeDIBSection(globalBackbuffer, width, height);
            OutputDebugStringA("WM_SIZE\n");
        } break;

        case WM_DESTROY: {
            //TODO(moosichu) handle this as an error
            gameRunning = false;
        } break;

        case WM_CLOSE: {
            //TODO(moosichu) handle this
            gameRunning = false;
        } break;

        case WM_ACTIVATEAPP: {
            OutputDebugStringA("WM_ACTIVATE_APP\n");
        } break;

        case WM_PAINT: {
            PAINTSTRUCT paint;  //A structure for painting to the window
            HDC deviceContext = BeginPaint(windowHandle, &paint);
            int height = paint.rcPaint.bottom - paint.rcPaint.top;
            int width = paint.rcPaint.right - paint.rcPaint.left;
            int x = paint.rcPaint.left;
            int y = paint.rcPaint.top;

            RECT clientRect; //Area you can draw to
            GetClientRect(windowHandle, &clientRect);
            
            copyBufferToDisplayWindow(deviceContext, clientRect, globalBackbuffer, x, y, width, height);
            EndPaint(windowHandle, &paint);
        } break;

        default: {
            result = DefWindowProc(windowHandle, message, wParam, lParam);  //Have to say we haven't handled any of the other messages (so windows can handle default behaviour)
        } break;
     }
    return result;
}

/**
 * The main method that is called by Windows.
 */
int WinMain(
    HINSTANCE instanceHandle,          //Handle to the current instance of the application
    HINSTANCE previousInstanceHandle,  //Handle to the previous instance of the application
    LPSTR commandLine,                 //The commandline for the application excluding the program name
    int showCode)                      //Controls how the window will be shown
{
    //Initialise everything in windowClass to 0!
    WNDCLASS windowClass = {};

    windowClass.style = CS_HREDRAW|CS_VREDRAW;            //Defines the style of the window
    windowClass.lpfnWndProc = mainWindowCallback;         //A pointer to a function that handles window events
    windowClass.hInstance = instanceHandle;               //What instance is setting the window.
//    windowClass.hIcon = ;                               //Set an icon
    windowClass.lpszClassName = "GameEngineWindowClass";  //Name for our windows class

    if(RegisterClass(&windowClass)) {
        HWND windowHandle = CreateWindowEx(
            0,                               //ExtendedWindowStyle
            windowClass.lpszClassName,       //The classname from the windowClass struct
            "Game Engine",                   //TODO(moosichu) Make this configurable from scripting (window title)
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,  //Window Style
            CW_USEDEFAULT,                   //x-coordinate
            CW_USEDEFAULT,                   //y-coordinate
            CW_USEDEFAULT,                   //The window width
            CW_USEDEFAULT,                   //The window height
            0,                               //Define parent window (not needed)
            0,                               //The menu we want to have
            instanceHandle,
            0                                //An optional parameter you can pass to the window

                                           );
        if(windowHandle) {  //Make sure it's not NULL
            
            //Loop through mesages that have been sent to our window
            int xOffset = 0;
            int yOffset = 0;
            
            gameRunning = true;
            while(gameRunning) {
                MSG message;
                
                //Go through all the windows messages
                while(PeekMessage(&message, 0, 0, 0, PM_REMOVE)) { //Returns 0 if WM_QUIT, -1 if some kind of error
                    if(message.message == WM_QUIT) {
                        gameRunning = false;
                    }
                    
                    TranslateMessage(&message);  //Takes and processes a message (for our use)
                    DispatchMessageA(&message);  //No point in checking these as there isn't much we can do if they fail
                }

                //renderWeirdGradient(globalBackbuffer, xOffset, yOffset);
                rayTracerMain(globalBackbuffer);
                HDC deviceContext = GetDC(windowHandle);
                RECT clientRect;
                GetClientRect(windowHandle, &clientRect);
                int windowWidth = clientRect.right - clientRect.left;
                int windowHeight = clientRect.bottom - clientRect.top;

                copyBufferToDisplayWindow(deviceContext, clientRect, globalBackbuffer, 0, 0, windowWidth, windowHeight);
                ReleaseDC(windowHandle, deviceContext);

                xOffset++;
            }
        } else { //windowHandle is NULL
            //TODO(moosichu) Log this error
        }
    } else { //RegisterClass() returned NULL
        //TODO(moosichu)  Log this error
    }
    return(0);
}
