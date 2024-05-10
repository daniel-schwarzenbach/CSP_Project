#!/bin/bash

#SBATCH --nodes=48                   
#SBATCH --ntasks=48              
#SBATCH --ntasks-per-node=1 
#SBATCH --mem-per-cpu=2000
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=04:00:00

mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.66
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.67
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.69
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.70
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 2
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 2.33
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 2.37