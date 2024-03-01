@echo off

if not exist build mkdir build

rem generate cmake files
cmake -O . -B ./build -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON

rem compile project
cd build && cmake --build .
echo compilation finished!