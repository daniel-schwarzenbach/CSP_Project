#!/bin/bash

#check build dir
if [ ! -d build ]
then
    mkdir build
fi

#generate cmake files
cmake -O . -B ./build -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=OFF

# compile project
cd build && cmake --build .
echo compilation finished!
