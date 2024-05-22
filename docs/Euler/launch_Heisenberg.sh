#!/bin/bash

#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1    #Requesting 1 CPU for each job
#SBATCH --mem-per-cpu=2000
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=72:00:00

srun .././programs/HeisenbergMPI $1 $2