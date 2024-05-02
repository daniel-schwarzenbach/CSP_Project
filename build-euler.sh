#!/bin/bash

#check build dir
if [ ! -d build ]
then
    mkdir build
fi

#generate cmake files
module load gcc/11.4.0 openmpi/4.1.4 eigen/3.4.0 cmake/3.26.3 openblas/0.3.20 fftw/3.3.10
cmake -O . -B ./build -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=OFF -DIPPL=OFF -Wno-dev


# compile project
cd build && cmake --build . && ctest