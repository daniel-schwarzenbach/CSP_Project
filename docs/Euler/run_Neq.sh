############ load dynamic libraries
module load gcc/11.4.0 eigen/3.4.0 cmake/3.26.3 openblas/0.3.20 fftw/3.3.10  cuda/12.1.1 openmpi/4.1.4

############ make output dire
if [ ! -d output ]
then
    mkdir output
fi

############ Decompress input.zip

# Define the directory where input is decompressed
output_dir="./input"

# Check if the output directory already exists
if [ -d "$output_dir" ]; then
    echo "The directory $output_dir already exists. Assuming the zip has been decompressed."
else
    echo "Decompressing the zip file into $output_dir..."
    # Create the directory
    mkdir -p "$output_dir"
    # Decompress the zip file into the directory
    unzip -q ./input.zip -d "$output_dir"
    echo "Decompression complete."
fi

########### run batch launchscript
cd output && sbatch ../launch_Neq.sh