#include <Wolff/wolff_ghost.h++>
#include <Wolff/wolff.h++>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timekeeper.h++>
#include <Measure/Observables.h++>
#include <fstream>
#include <vector>
#include <string>
#include <Metropolis/adaptive_metropolis.h++>
#include <Metropolis/metropolis.h++>

// temperature
//F64 T = 2.0;
flt J = 1.0;
flt Time = 3600.0;
Spin h = Spin(0.0,0.0,0.0);

//Compute means and std to determine the moments of magnetisation and Energy (e.g. specific heat, binder cumulant)

//Determine N_eq for different temperatures and take linear ramp Eq. 42, 43
// for i in Temperature...


// void loop_algorithm(algorithm, quantity, T){

//     return 0;
// }


void loop_magnetisation(Lattice& lattice){
    Vector3 magnetisation = measure::get_magnetization(lattice);
    flt energy = measure::get_energy(lattice, h, J);
    magnetisations[j] = magnetisation;
    flt mag = magnetisation.norm();
    energies[j] = energy;

}


void loop_algorithm(algorithm, quantity, std::vector<double> temperatures, const uint seed, int Nmax, int Ns){
    
    //              --- random seed
    rng::set_seed(seed);
    
    int number_of_steps = round(Nmax/Ns);
    
    return 0;
}

int main()
{

    for (size_t i = 0; i < temperatures.size(); ++i) {
        Lattice lattice = Lattice::random_lattice(8, 8, 8);
        //Plot magnetisation and energy as a function of steps to determine N_eq (until equilibrium)
        Array<Vector3> magnetisations(number_of_steps + 1); 
        Array<F64> energies(number_of_steps + 1); 

        double temperature = temperatures[i]; //Get temperature

        cout << "Simulating at temperature: " << temperature << endl;

        std::string filename = "mag_and_energy_" + to_string(temperature) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"

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
            energies[j] = energy;

            metropolis(lattice, temperature, J, Time, Ns);

            outFile << Ns*j << " " << mag << " " << energy << std::endl; //Write current total step number, mag and E into file
        }
        
        outFile.close();
    
    }

    return 0;
}

