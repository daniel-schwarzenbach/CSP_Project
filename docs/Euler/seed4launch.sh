#!/bin/bash

#SBATCH --nodes=192                   
#SBATCH --ntasks=192                 
#SBATCH --ntasks-per-node=1 
#SBATCH --mem-per-cpu=2000
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=20:00:00

mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 1.65
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 1.8
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 3
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 10
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 100
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 1.53
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 1.57
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 1.6
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 1.37
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 1.4
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 1.43
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 0.5
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 1
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPI 1.2