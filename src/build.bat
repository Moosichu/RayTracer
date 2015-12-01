@echo off
mkdir ..\bin
pushd ..\bin
cl /Zi /FC ..\src\win_main.cpp ..\src\color.cpp ..\src\line_drawer.cpp ..\src\ray_tracer.cpp user32.lib gdi32.lib
popd