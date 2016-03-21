mkdir ../bin
clang++-3.6 -std=c++11 sdl_main.cpp line_drawer.cpp ray_tracer.cpp color.cpp -o ../bin/main `sdl2-config --cflags --libs`
