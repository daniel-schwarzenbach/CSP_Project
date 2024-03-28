if [ ! -d build ]
then
    mkdir build
fi

cd build
../ippl-build-scripts/999-build-everything -t mixed --enable-openmp --enable-serial --kokkos --heffte --ippl --export --arch=C++20