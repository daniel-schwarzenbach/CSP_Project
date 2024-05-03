if [ ! -d build ]
then
    mkdir build
fi

cd build
../ippl-build-scripts/999-build-everything -t serial -k -f -i -u 