#!/bin/bash

#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8    #Requesting 8 CPU for each job
#SBATCH --mem-per-cpu=6000
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=72:00:00

srun .././programs/Get_Speed_Up_Data