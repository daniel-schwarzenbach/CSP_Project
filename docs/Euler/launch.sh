#!/bin/bash 

#SBATCH -n 100 
#SBATCH --time=00:10:00 
#SBATCH --job-name=test 
#SBATCH --mem-per-cpu=512 
#SBATCH --output=file.out 
#SBATCH --error=file.err

mpirun ../../build/HeisenbergMPI