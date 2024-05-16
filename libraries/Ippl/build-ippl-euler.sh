env2lmod

module purge

module load gcc/11.4.0 cmake/3.26.3 cuda/12.1.1 openmpi/4.1.4

#check build dir
if [ ! -d build ]
then
mkdir build
fi

cd build && ../ippl-build-scripts/999-build-everything -t openmp -k -f -i -u