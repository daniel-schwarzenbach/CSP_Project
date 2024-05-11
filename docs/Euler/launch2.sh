#!/bin/bash

#SBATCH --nodes=48                   
#SBATCH --ntasks=48              
#SBATCH --ntasks-per-node=1 
#SBATCH --mem-per-cpu=2500
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=36:00:00

mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.42
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.44
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.45
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.46
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.48
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.49
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.51
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI 1.52