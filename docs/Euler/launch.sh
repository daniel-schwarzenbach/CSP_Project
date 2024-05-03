#!/bin/bash

#SBATCH --nodes=25                   
#SBATCH --ntasks=25                  
#SBATCH --ntasks-per-node=1         
#SBATCH --output=output.txt
#SBATCH --error=error.txt
#SBATCH --time=01:00:00

mpirun -bind-to core -map-by core -n 25 ../../build/HeisenbergMPI