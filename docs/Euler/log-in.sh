ssh -Y dschwarze@euler.ethz.ch

#New software stack
env2lmod

module load gcc/11.4.0 eigen/3.4.0 cmake/3.26.3 openblas/0.3.20 fftw/3.3.10  cuda/12.1.1 openmpi/4.1.4
#sbatch jobscript.sh

# module list
# module avaiable