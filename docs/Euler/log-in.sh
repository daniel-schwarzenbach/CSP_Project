ssh -Y dschwarze@euler.ethz.ch

#New software stack
env2lmod
module load gcc/11.4.0 openmpi/4.1.4 eigen/3.4.0 cmake/3.26.3

#sbatch jobscript.sh

#module avaiable