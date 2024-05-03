#include <Wolff/wolff.h++>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <fstream>
#include <vector>
#include <string>
#include <Metropolis/metropolis.h++>
#include <mpi/mpi.h>

std::vector<T> splitVector(const std::vector<T>& vec, int rank, int size) {
    // Calculate the size of each split
    size_t splitSize = vec.size() / size;
    size_t startIndex = rank * splitSize;
    size_t endIndex = (rank == size - 1) ? vec.size() : (startIndex + splitSize);

    // Handle the case where the vector cannot be evenly split
    if (vec.size() % size != 0 && rank == size - 1) {
        endIndex = vec.size();
    }

    // Create a sub-vector for the current rank
    std::vector<T> subVec(vec.begin() + startIndex, vec.begin() + endIndex);
    return subVec;
}

// temperature
//F64 T = 2.0;
flt const J = 1.0;
flt const& Time = 3600.0;
Spin const& h = Spin(0.0, 0.0, 1.0);
uint constexpr Seed = 42;

uint Lx = 32;
uint Ly = 32; 
uint Lz = 32; 


int main(int argc, char** argv)
{
    Array<double> temperatures = {0.01, 0.03, 0.1, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 2.0, 10.0};
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_Comm::MPI_COMM_WORLD;
    int rank;
    MPI_Comm_rank(comm, &rank);
    int size;
    MPI_Comm_size(comm, &size);


    rank 

    

    //              --- Lattice
    
    //dat::plot_lattice(lattice, "Wolff_start.png");

    //Define maximal number of steps, step width and number of iterations
    u64 Nmax = 100000;
    u64 Ns = 1000;
    u64 number_of_steps = round(Nmax/Ns); 

    //Define temperature array which we loop over

    std::vector<double> temperatures = {0.01, 0.03, 0.1, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 2.0, 10.0};
    //std::vector <double> temperatures = {0.01, };

    for (size_t i = 0; i < temperatures.size(); ++i) {
        Lattice lattice = Lattice::random_lattice(Lx, Ly, Lz);
        //Plot magnetisation and energy as a function of steps to determine N_eq (until equilibrium)
        Array<Vector3> magnetisations(number_of_steps + 1); 
        Array<flt> energies(number_of_steps + 1); 

        double temperature = temperatures[i]; //Get temperature

        cout << "Simulating at temperature: " << temperature << endl;

        std::string filename = "mag_and_energy_" + to_string(temperature) + "_8_wolff.txt"; //Create .txt file with name "mag_and_energy_T.txt"

        // Open the output file for writing with the given filename.
        // If the file is successfully opened:
        //   - Write a header indicating the temperature of the simulation.
        //   - Optionally, write additional simulation results if needed.
        //   - Print a message indicating that the output file is saved.
        // If the file cannot be opened, print an error message.

        std::ofstream outFile(filename);
        if (outFile.is_open()) {
            // Write output data to file
            outFile << "Simulation at temperature: " << temperature << endl;
            // Write additional simulation results if needed
            cout << "Output saved to " << filename << endl;
        } 
        else {
                cout << "Error: Unable to open file " << filename << " for writing." << endl;
            }

        //Run algorithm "number_of_steps" times and save energy and magnetisation for each step
        for (int j = 0; j < number_of_steps + 1; ++j){
            Vector3 magnetisation = measure::get_magnetization(lattice);
            flt energy = measure::get_energy(lattice, h, J);
            magnetisations[j] = magnetisation;
            flt mag = magnetisation.norm();
            flt mag_x = magnetisation[0];
            flt mag_y = magnetisation[1];
            flt mag_z = magnetisation[2];
            energies[j] = energy;
            wolff_ghost(lattice, temperature, J, Time, Ns, h);

            outFile << Ns*j << " " << mag << " " << mag_x << " " << mag_y << " " << mag_z << " " << energy << std::endl; //Write current total step number, mag and E into file
        }
        
        outFile.close();
    
    }