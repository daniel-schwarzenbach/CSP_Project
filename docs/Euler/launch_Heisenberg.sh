#!/bin/bash

#SBATCH --ntasks=1
#SBATCH --array=1-54     #Array with 54 Jobs, always 27 running in parallel
#SBATCH --cpus-per-task=1    #Requesting 1 CPU for each job
#SBATCH --mem-per-cpu=2500
#SBATCH --error=error.txt
#SBATCH --output=output.txt
#SBATCH --time=72:00:00

srun .././programs/HeisenbergMPI 0.01 $1
srun .././programs/HeisenbergMPI 0.1 $1
srun .././programs/HeisenbergMPI 0.3 $1
srun .././programs/HeisenbergMPI 0.75 $1
srun .././programs/HeisenbergMPI 1.0 $1
srun .././programs/HeisenbergMPI 1.1 $1
srun .././programs/HeisenbergMPI 1.2 $1
srun .././programs/HeisenbergMPI 1.3 $1
srun .././programs/HeisenbergMPI 1.31 $1
srun .././programs/HeisenbergMPI 1.32 $1
srun .././programs/HeisenbergMPI 1.33 $1
srun .././programs/HeisenbergMPI 1.34 $1
srun .././programs/HeisenbergMPI 1.35 $1
srun .././programs/HeisenbergMPI 1.36 $1
srun .././programs/HeisenbergMPI 1.37 $1
srun .././programs/HeisenbergMPI 1.38 $1
srun .././programs/HeisenbergMPI 1.39 $1
srun .././programs/HeisenbergMPI 1.40 $1
srun .././programs/HeisenbergMPI 1.41 $1
srun .././programs/HeisenbergMPI 1.42 $1
srun .././programs/HeisenbergMPI 1.43 $1
srun .././programs/HeisenbergMPI 1.44 $1
srun .././programs/HeisenbergMPI 1.45 $1
srun .././programs/HeisenbergMPI 1.46 $1
srun .././programs/HeisenbergMPI 1.47 $1
srun .././programs/HeisenbergMPI 1.48 $1
srun .././programs/HeisenbergMPI 1.49 $1
srun .././programs/HeisenbergMPI 1.50 $1
srun .././programs/HeisenbergMPI 1.51 $1
srun .././programs/HeisenbergMPI 1.52 $1
srun .././programs/HeisenbergMPI 1.53 $1
srun .././programs/HeisenbergMPI 1.54 $1
srun .././programs/HeisenbergMPI 1.55 $1
srun .././programs/HeisenbergMPI 1.56 $1
srun .././programs/HeisenbergMPI 1.57 $1
srun .././programs/HeisenbergMPI 1.58 $1
srun .././programs/HeisenbergMPI 1.59 $1
srun .././programs/HeisenbergMPI 1.60 $1
srun .././programs/HeisenbergMPI 1.61 $1
srun .././programs/HeisenbergMPI 1.62 $1
srun .././programs/HeisenbergMPI 1.63 $1
srun .././programs/HeisenbergMPI 1.64 $1
srun .././programs/HeisenbergMPI 1.65 $1
srun .././programs/HeisenbergMPI 1.66 $1
srun .././programs/HeisenbergMPI 1.67 $1
srun .././programs/HeisenbergMPI 1.68 $1
srun .././programs/HeisenbergMPI 1.69 $1
srun .././programs/HeisenbergMPI 1.70 $1
srun .././programs/HeisenbergMPI 1.8 $1
srun .././programs/HeisenbergMPI 1.9 $1
srun .././programs/HeisenbergMPI 2.0 $1
srun .././programs/HeisenbergMPI 3.0 $1
srun .././programs/HeisenbergMPI 5.0 $1
srun .././programs/HeisenbergMPI 10.0 $1