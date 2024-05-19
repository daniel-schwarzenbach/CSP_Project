#!/bin/bash

#SBATCH --nodes=12                  
#SBATCH --ntasks=12             
#SBATCH --ntasks-per-node=1 
#SBATCH --mem-per-cpu=2500
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=36:00:00

mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.70 32
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 2.33 32
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 2.37 32