env2lmod
module purge
module load gcc/11.4.0 cmake/3.26.3 cuda/12.1.1 openmpi/4.1.4

git clone https://github.com/IPPL-framework/ippl.git

cd ippl
mkdir build
cd build

cmake .. -DCMAKE_CXX_STANDARD=20 -DIPPL_PLATFORMS=OPENMP -DENABLE_SOLVERS=ON -DENABLE_FFT=ON -DENABLE_ALPINE=ON
make