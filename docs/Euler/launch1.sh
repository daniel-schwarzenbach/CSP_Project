#!/bin/bash

#SBATCH --nodes=48                   
#SBATCH --ntasks=48              
#SBATCH --ntasks-per-node=1 
#SBATCH --mem-per-cpu=2500
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=20:00:00

mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 0.3
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 0.75
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.31
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.32
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.34
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.35
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.36
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.38
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.39
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.41