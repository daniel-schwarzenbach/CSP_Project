############ load dynamic libraries
module load gcc/11.4.0 eigen/3.4.0 cmake/3.26.3 openblas/0.3.20 fftw/3.3.10  cuda/12.1.1 openmpi/4.1.4 #libjpeg/9e

############ make output dire
if [ ! -d output ]
then
    mkdir output
fi

########### run batch launchscript
cd output && sbatch ../launch_Heisenberg.sh $1