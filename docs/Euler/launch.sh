#!/bin/bash 

#SBATCH -n 25 
#SBATCH --time=00:10:00 
#SBATCH --job-name=test 
#SBATCH --mem-per-cpu=512 
#SBATCH --output=output.out 
#SBATCH --error=file.err

srun ../../build/HeisenbergMPI