if [ ! -d build ]
then
    mkdir build
fi

module load gcc/11.4.0 openmpi/4.1.4 eigen/3.4.0 cmake/3.26.3 openblas/0.3.20 fftw/3.3.10
cd build
../ippl-build-scripts/999-build-everything -t openmp -k -f -i -u