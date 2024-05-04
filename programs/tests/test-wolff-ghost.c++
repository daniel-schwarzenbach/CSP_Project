#include <Wolff/wolff.h++>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <fstream>
#include <vector>
#include <string>
#include <Metropolis/metropolis.h++>

// temperature
//F64 T = 2.0;
flt J = 1.0;
flt Time = 3600.0;
Spin h = Spin(0.0, 0.0, 0.0);
Spin k = Spin(0.0, 0.0, 0.0);


int main()
{
    //              --- random seed
    const uint seed = 42;
    rng::set_seed(seed);

    int Lx = 8;
    int Ly = Lx; 
    int Lz = Ly; 

    //              --- Lattice
    
    //dat::plot_lattice(lattice, "Wolff_start.png");

    //Define maximal number of steps, step width and number of iterations
    // u64 Nmax = 10000000000;
    // u64 Ns = 1000000;

    u64 Nmax = 1000000;
    u64 Ns = 1000;

    u64 number_of_steps = round(Nmax/Ns); 

    int nruns = 1;

    //Define temperature array which we loop over

    //std::vector<double> temperatures = {0.01, 0.03, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 2.0, 5.0, 10.0};
    std::vector <double> temperatures = {10.01};

    for (size_t i = 0; i < temperatures.size(); ++i) {
        Lattice lattice = Lattice::random_lattice(Lx, Ly, Lz);
        //Plot magnetisation and energy as a function of steps to determine N_eq (until equilibrium)
        Array<Vector3> magnetisations(number_of_steps + 1); 
        Array<flt> energies(number_of_steps + 1); 

            double temperature = temperatures[i]; //Get temperature

            cout << "Simulating at temperature: " << temperature << endl;

            std::string filename = "mag_and_energy_metropolis_T_" + to_string(temperature) + "_L_" + to_string(Lx)+ "_no_mag_field_" + "_run_" + to_string(a) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"

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
                flt energy = measure::get_energy(lattice, h, k, J);
                magnetisations[j] = magnetisation;
                flt mag = magnetisation.norm();
                flt mag_x = magnetisation[0];
                flt mag_y = magnetisation[1];
                flt mag_z = magnetisation[2];
                energies[j] = energy;
                wolff_ghost(lattice, temperature, J, Time, Ns, h);

                outFile << Ns*j << " " << mag << " " << mag_z << " " << energy << std::endl; //Write current total step number, mag and E into file
                cout << "nsteps: " << Ns*j << "mag "  << mag << " mag_z: " << mag_z << ", energy: " << energy << endl; 
            }
            outFile.close();
        
        }
    }

    // for (int a = 0; a < nruns; ++a){
    //     for (size_t i = 0; i < temperatures.size(); ++i) {
    //         Lattice lattice = Lattice::random_lattice(Lx, Ly, Lz);
    //         //Plot magnetisation and energy as a function of steps to determine N_eq (until equilibrium)
    //         Array<Vector3> magnetisations(number_of_steps + 1); 
    //         Array<F64> energies(number_of_steps + 1); 

    //         double temperature = temperatures[i]; //Get temperature

    //         cout << "Simulating at temperature: " << temperature << endl;

    //         std::string filename = "mag_and_energy_adaptive_metropolis_T_" + to_string(temperature) + "_L_" + to_string(Lx)+ "_no_mag_field_" + "_run_" + to_string(a) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"

    //         // Open the output file for writing with the given filename.
    //         // If the file is successfully opened:
    //         //   - Write a header indicating the temperature of the simulation.
    //         //   - Optionally, write additional simulation results if needed.
    //         //   - Print a message indicating that the output file is saved.
    //         // If the file cannot be opened, print an error message.

    //         std::ofstream outFile(filename);
    //         if (outFile.is_open()) {
    //             // Write output data to file
    //             outFile << "Simulation at temperature: " << temperature << endl;
    //             // Write additional simulation results if needed
    //             cout << "Output saved to " << filename << endl;
    //         } 
    //         else {
    //                 cout << "Error: Unable to open file " << filename << " for writing." << endl;
    //             }

    //         //Run algorithm "number_of_steps" times and save energy and magnetisation for each step
    //         for (int j = 0; j < number_of_steps + 1; ++j){
    //             Vector3 magnetisation = measure::get_magnetization(lattice);
    //             flt energy = measure::get_energy(lattice, h, k, J);
    //             magnetisations[j] = magnetisation;
    //             flt mag = magnetisation.norm();
    //             flt mag_x = magnetisation[0];
    //             flt mag_y = magnetisation[1];
    //             flt mag_z = magnetisation[2];
    //             energies[j] = energy;
    //             adaptive_metropolis(lattice, temperature, J, Time, Ns, h);

    //             outFile << Ns*j << " " << mag << " " << mag_x << " " << mag_y << " " << mag_z << " " << energy << std::endl; //Write current total step number, mag and E into file
    //         }
            
    //         outFile.close();
        
    //     }
    // }

    // u64 Nmax = 1000000;
    // u64 Ns = 10000;

    // //u64 Nmax = 1000000;
    // //u64 Ns = 1000;

    // u64 number_of_steps = round(Nmax/Ns); 

    // //Define temperature array which we loop over

    // std::vector<double> temperatures = {0.01, 0.03, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 2.0, 5.0, 10.0};
    // //std::vector <double> temperatures = {0.1, 0.2};

    // for (int a = 0; a < 100; ++a){
    //     for (size_t i = 0; i < temperatures.size(); ++i) {
    //         Lattice lattice = Lattice::random_lattice(Lx, Ly, Lz);
    //         //Plot magnetisation and energy as a function of steps to determine N_eq (until equilibrium)
    //         Array<Vector3> magnetisations(number_of_steps + 1); 
    //         Array<F64> energies(number_of_steps + 1); 

    //         double temperature = temperatures[i]; //Get temperature

    //         cout << "Simulating at temperature: " << temperature << endl;

    //         std::string filename = "mag_and_energy_wolff_T_" + to_string(temperature) + "_Nmax_" + "" + "_L_" + to_string(Lx)+ "_no_mag_field_" + "_run_" + to_string(a) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"

    //         // Open the output file for writing with the given filename.
    //         // If the file is successfully opened:
    //         //   - Write a header indicating the temperature of the simulation.
    //         //   - Optionally, write additional simulation results if needed.
    //         //   - Print a message indicating that the output file is saved.
    //         // If the file cannot be opened, print an error message.

    //         std::ofstream outFile(filename);
    //         if (outFile.is_open()) {
    //             // Write output data to file
    //             outFile << "Simulation at temperature: " << temperature << endl;
    //             // Write additional simulation results if needed
    //             cout << "Output saved to " << filename << endl;
    //         } 
    //         else {
    //                 cout << "Error: Unable to open file " << filename << " for writing." << endl;
    //             }

    //         //Run algorithm "number_of_steps" times and save energy and magnetisation for each step
    //         for (int j = 0; j < number_of_steps + 1; ++j){
    //             Vector3 magnetisation = measure::get_magnetization(lattice);
    //             flt energy = measure::get_energy(lattice, h, k, J);
    //             magnetisations[j] = magnetisation;
    //             flt mag = magnetisation.norm();
    //             flt mag_x = magnetisation[0];
    //             flt mag_y = magnetisation[1];
    //             flt mag_z = magnetisation[2];
    //             energies[j] = energy;
    //             metropolis(lattice, temperature, J, Time, Ns, h);

    //             outFile << Ns*j << " " << mag << " " << mag_x << " " << mag_y << " " << mag_z << " " << energy << std::endl; //Write current total step number, mag and E into file
    //         }
            
    //         outFile.close();
        
    //     }
    
    
    //Compute means and std to determine the moments of magnetisation and Energy (e.g. specific heat, binder cumulant)

    //Determine N_eq for different temperatures and take linear ramp Eq. 42, 43
    // for i in Temperature...


    //ASSUMING 

    //             --- wolff-ghost
    // cout << "running wolff ghost ..." << endl;
    // wolff_ghost(lattice, T, J, Time, maxUint);

    // dat::plot_lattice(lattice, "Wolff_ghost_end.png");

    // rng::set_seed(seed);
    // lattice.randomize();

    // dat::plot_lattice(lattice, "Wolff_start_compare.png");

    // wolff(lattice, T, J, Time, maxUint);

    // dat::plot_lattice(lattice, "Wolff_end.png")
    return 0;
}