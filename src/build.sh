mkdir ../bin
emcc -std=c++11 sdl_main.cpp color.cpp line_drawer.cpp ray_tracer.cpp -o ../bin/main.html -s USE_SDL=2
