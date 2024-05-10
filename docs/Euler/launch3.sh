#!/bin/bash

#SBATCH --nodes=48                   
#SBATCH --ntasks=48              
#SBATCH --ntasks-per-node=1 
#SBATCH --mem-per-cpu=2000
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=04:00:00

mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.54
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.55
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.56
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.58
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.59
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.61
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.64