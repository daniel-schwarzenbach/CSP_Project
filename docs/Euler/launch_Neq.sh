#!/bin/bash

#SBATCH --nodes=48                   
#SBATCH --ntasks=48              
#SBATCH --ntasks-per-node=1 
#SBATCH --mem-per-cpu=2000
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=04:00:00

mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI_Neq 0.5
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI_Neq 1
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI_Neq 1.8
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI_Neq 3.0
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI_Neq 10