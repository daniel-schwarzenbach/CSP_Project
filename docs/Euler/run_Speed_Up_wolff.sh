############ load dynamic libraries
module load gcc/11.4.0 eigen/3.4.0 cmake/3.26.3 openblas/0.3.20 fftw/3.3.10  cuda/12.1.1 openmpi/4.1.4

############ make output dire
if [ ! -d output_wolff ]
then
    mkdir output_wolff
fi

########### run batch launchscript
cd output_wolff && sbatch ../launch_Speed_Up_wolff.sh