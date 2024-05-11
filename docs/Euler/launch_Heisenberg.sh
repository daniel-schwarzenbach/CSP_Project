#!/bin/bash

#SBATCH --nodes=48                   
#SBATCH --ntasks=48              
#SBATCH --ntasks-per-node=1 
#SBATCH --mem-per-cpu=2000
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=24:00:00

mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 0.5
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.5
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 2