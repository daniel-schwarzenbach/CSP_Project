#!/bin/bash

#SBATCH --nodes=48                   
#SBATCH --ntasks=48              
#SBATCH --ntasks-per-node=1 
#SBATCH --mem-per-cpu=2000
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=04:00:00

mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI_speed_up 0.01
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI_speed_up 0.7
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI_speed_up 2
mpirun -bind-to core -map-by core -n 12 .././programs/HeisenbergMPI_speed_up 20