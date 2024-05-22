############ load dynamic libraries
module load gcc/11.4.0 eigen/3.4.0 cmake/3.26.3 openblas/0.3.20 fftw/3.3.10  cuda/12.1.1 openmpi/4.1.4 #libjpeg/9e

############ make output dire
if [ ! -d output_$1 ]
then
    mkdir output_$1
fi

########### run batch launchscript
cd output_$1 

sbatch ../launch_Heisenberg.sh 0.01 $1
sbatch ../launch_Heisenberg.sh 0.1 $1
sbatch ../launch_Heisenberg.sh 0.3 $1
sbatch ../launch_Heisenberg.sh 0.75 $1
sbatch ../launch_Heisenberg.sh 1.0 $1
sbatch ../launch_Heisenberg.sh 1.1 $1
sbatch ../launch_Heisenberg.sh 1.2 $1
sbatch ../launch_Heisenberg.sh 1.3 $1
sbatch ../launch_Heisenberg.sh 1.31 $1
sbatch ../launch_Heisenberg.sh 1.32 $1
sbatch ../launch_Heisenberg.sh 1.33 $1
sbatch ../launch_Heisenberg.sh 1.34 $1
sbatch ../launch_Heisenberg.sh 1.35 $1
sbatch ../launch_Heisenberg.sh 1.36 $1
sbatch ../launch_Heisenberg.sh 1.37 $1
sbatch ../launch_Heisenberg.sh 1.38 $1
sbatch ../launch_Heisenberg.sh 1.39 $1
sbatch ../launch_Heisenberg.sh 1.40 $1
sbatch ../launch_Heisenberg.sh 1.41 $1
sbatch ../launch_Heisenberg.sh 1.42 $1
sbatch ../launch_Heisenberg.sh 1.43 $1
sbatch ../launch_Heisenberg.sh 1.44 $1
sbatch ../launch_Heisenberg.sh 1.45 $1
sbatch ../launch_Heisenberg.sh 1.46 $1
sbatch ../launch_Heisenberg.sh 1.47 $1
sbatch ../launch_Heisenberg.sh 1.48 $1
sbatch ../launch_Heisenberg.sh 1.49 $1
sbatch ../launch_Heisenberg.sh 1.50 $1
sbatch ../launch_Heisenberg.sh 1.51 $1
sbatch ../launch_Heisenberg.sh 1.52 $1
sbatch ../launch_Heisenberg.sh 1.53 $1
sbatch ../launch_Heisenberg.sh 1.54 $1
sbatch ../launch_Heisenberg.sh 1.55 $1
sbatch ../launch_Heisenberg.sh 1.56 $1
sbatch ../launch_Heisenberg.sh 1.57 $1
sbatch ../launch_Heisenberg.sh 1.58 $1
sbatch ../launch_Heisenberg.sh 1.59 $1
sbatch ../launch_Heisenberg.sh 1.60 $1
sbatch ../launch_Heisenberg.sh 1.61 $1
sbatch ../launch_Heisenberg.sh 1.62 $1
sbatch ../launch_Heisenberg.sh 1.63 $1
sbatch ../launch_Heisenberg.sh 1.64 $1
sbatch ../launch_Heisenberg.sh 1.65 $1
sbatch ../launch_Heisenberg.sh 1.66 $1
sbatch ../launch_Heisenberg.sh 1.67 $1
sbatch ../launch_Heisenberg.sh 1.68 $1
sbatch ../launch_Heisenberg.sh 1.69 $1
sbatch ../launch_Heisenberg.sh 1.70 $1
sbatch ../launch_Heisenberg.sh 1.8 $1
sbatch ../launch_Heisenberg.sh 1.9 $1
sbatch ../launch_Heisenberg.sh 2.0 $1
sbatch ../launch_Heisenberg.sh 3.0 $1
sbatch ../launch_Heisenberg.sh 5.0 $1
sbatch ../launch_Heisenberg.sh 10.0 $1