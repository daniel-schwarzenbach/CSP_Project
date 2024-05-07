#!/bin/bash

#SBATCH --nodes=48                   
#SBATCH --ntasks=48                  
#SBATCH --ntasks-per-node=1 
#SBATCH --mem-per-cpu=2000
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=20:00:00



mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPIwolff 10
mpirun -bind-to core -map-by core -n 48 .././programs/HeisenbergMPIwolff 1.6