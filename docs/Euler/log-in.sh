ssh -Y dschwarze@euler.ethz.ch

#New software stack
env2lmod
module load new gcc/4.8.2 cmake/3.11.4
module load gcc/8.2.0 openmpi/4.1.4 eigen/3.3.9 cmake/3.5.2

#sbatch jobscript.sh