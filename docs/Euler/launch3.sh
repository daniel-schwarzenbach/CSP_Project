#!/bin/bash

#SBATCH --nodes=12                   
#SBATCH --ntasks=12             
#SBATCH --ntasks-per-node=1 
#SBATCH --mem-per-cpu=2500
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=36:00:00

mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.54
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.55
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.56
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.58
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.59
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.61
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.62
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.63
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.64