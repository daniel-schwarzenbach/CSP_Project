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
#include <iostream>
#include <assert.h>

std::basic_ostream<char>& operator<<(std::basic_ostream<char>& of, std::vector<double> const& array){
    for (double x : array){
        of << x << " ";
    }
    return of;
    //of << "\n";
}


// Loop over specified system at different temperatures
// function defintion:
//  Inputs: array of temperatures, h, k, 
//  Output: void, prints results to file

void simulation(std::vector<double> temperatures, Spin h, Spin k, int Lx, int Ly, int Lz, float J, int Nmax, int Ns, float Time){
    //      METROPOLIS
    const uint seed = 42;
    rng::set_seed(seed);
    int number_of_steps = round(Nmax/Ns);

    std::string filename_metro = "Metropolis, Lattice Size " + to_string(Lx) + "x" + to_string(Ly) + "x" + to_string(Lz) + ", h_z " + to_string(h(3)) + ", k_z " + to_string(k(3)) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"


    std::ofstream outFile(filename_metro);
    if (outFile.is_open()) {
        // Write output data to file
        outFile << "Metropolis" << endl << "Lattice = " <<  Lx << "," << Ly << "," << Lz << endl << "h = " << h(0)<< "," << h(1) << "," << h(2) << endl
        << "k = " << k(0)<< "," << k(1) << "," << k(2) << endl << "Ns = " << Ns << endl << "Nmax = " << Nmax << endl << "No_of_datapoints = " << number_of_steps << endl;
        for (float temp: temperatures){outFile << temp << " ";}
        outFile << endl << "====" << endl;
        // Write additional simulation results if needed
        cout << "Output saved to " << filename_metro << endl;
    } 
    else {
        cout << "Error: Unable to open file " << filename_metro << " for writing." << endl;
    }

    for (size_t i = 0; i < temperatures.size(); ++i) {
        Lattice lattice = Lattice::random_lattice(Lx, Ly, Lz);
        //Plot magnetisation and energy as a function of steps to determine N_eq (until equilibrium)
        Array<Vector3> magnetisations(number_of_steps + 1); 
        Array<F64> energies(number_of_steps + 1); 

        double temperature = temperatures[i]; //Get temperature

        cout << "Simulating at temperature: " << temperature << endl;

        std::vector<std::vector<double>> newData;
        outFile << to_string(temperature) << " M " << "Mz " << "E " << endl;

        //Run algorithm "number_of_steps" times and save energy and magnetisation for each step
        for (int j = 0; j < number_of_steps + 1; ++j){
            Vector3 magnetisation = measure::get_magnetization(lattice);
            flt energy = measure::get_energy(lattice, h, k, J);
            magnetisations[j] = magnetisation;
            flt mag = magnetisation.norm();
            flt mag_z = magnetisation(2);
            energies[j] = energy;

            metropolis(lattice, temperature, J, Time, Ns, h, k, MoveType::SmallStep);
            outFile << Ns*j << " " << mag << " " << mag_z << " "<< energy;
            if(j != number_of_steps) {outFile << std::endl;}
        }
        if(i + 1 != temperatures.size()){outFile << endl << "====" << endl;}
    }

    outFile.close();

    //        WOLFF

    std::string filename_wolff = "Wolff, Lattice Size " + to_string(Lx) + "x" + to_string(Ly) + "x" + to_string(Lz) + ", h_z " + to_string(h(3)) + ", k_z " + to_string(k(3)) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"


    std::ofstream outFile1(filename_wolff);
    if (outFile1.is_open()) {
        // Write output data to file
        outFile1 << "Wolff" << endl << "Lattice = " <<  Lx << "," << Ly << "," << Lz << endl << "h = " << h(0)<< "," << h(1) << "," << h(2) << endl
        << "k = " << k(0)<< "," << k(1) << "," << k(2) << endl << "Ns = " << Ns << endl << "Nmax = " << Nmax << endl << "No_of_datapoints = " << number_of_steps << endl;
        for (float temp: temperatures){outFile1 << temp << " ";}
        outFile1 << endl << "====" << endl;
        // Write additional simulation results if needed
        cout << "Output saved to " << filename_wolff << endl;
    } 
    else {
        cout << "Error: Unable to open file " << filename_wolff << " for writing." << endl;
    }

    for (size_t i = 0; i < temperatures.size(); ++i) {
        Lattice lattice = Lattice::random_lattice(Lx, Ly, Lz);
        //Plot magnetisation and energy as a function of steps to determine N_eq (until equilibrium)
        Array<Vector3> magnetisations(number_of_steps + 1); 
        Array<F64> energies(number_of_steps + 1); 

        double temperature = temperatures[i]; //Get temperature

        cout << "Simulating at temperature: " << temperature << endl;

        outFile1 << to_string(temperature) << " M " << "Mz " << "E " << endl;
        std::vector<std::string> data(number_of_steps);

        //Run algorithm "number_of_steps" times and save energy and magnetisation for each step
        for (int j = 0; j < number_of_steps + 1; ++j){
            Vector3 magnetisation = measure::get_magnetization(lattice);
            flt energy = measure::get_energy(lattice, h, k, J);
            magnetisations[j] = magnetisation;
            flt mag = magnetisation.norm();
            flt mag_z = magnetisation(2);
            energies[j] = energy;

            wolff(lattice, temperature, J, Time, Ns);
            outFile1 << Ns*j << " " << mag << " " << mag_z << " "<< energy;
            if(j != number_of_steps) {outFile1 << std::endl;}
        }
        if(i + 1 != temperatures.size()){outFile1 << endl << "====" << endl;}
    }

    outFile1.close();

 
    //        ADAPTIVE METROPOLIs

    std::string filename_adametro = "Adaptive Metropolis, Lattice Size " + to_string(Lx) + "x" + to_string(Ly) + "x" + to_string(Lz) + ", h_z " + to_string(h(3)) + ", k_z " + to_string(k(3)) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"


    std::ofstream outFile2(filename_adametro);
    if (outFile2.is_open()) {
        // Write output data to file
        outFile2 << "Adaptive Metropolis" << endl << "Lattice = " <<  Lx << "," << Ly << "," << Lz << endl << "h = " << h(0)<< "," << h(1) << "," << h(2) << endl
        << "k = " << k(0)<< "," << k(1) << "," << k(2) << endl << "Ns = " << Ns << endl << "Nmax = " << Nmax << endl << "No_of_datapoints = " << number_of_steps << endl;
        for (float temp: temperatures){outFile2 << temp << " ";}
        outFile2 << endl << "====" << endl;
        // Write additional simulation results if needed
        cout << "Output saved to " << filename_adametro << endl;
    } 
    else {
        cout << "Error: Unable to open file " << filename_adametro << " for writing." << endl;
    }

    for (size_t i = 0; i < temperatures.size(); ++i) {
        Lattice lattice = Lattice::random_lattice(Lx, Ly, Lz);
        //Plot magnetisation and energy as a function of steps to determine N_eq (until equilibrium)
        Array<Vector3> magnetisations(number_of_steps + 1); 
        Array<F64> energies(number_of_steps + 1); 

        double temperature = temperatures[i]; //Get temperature

        cout << "Simulating at temperature: " << temperature << endl;

        outFile2 << to_string(temperature) << " M " << "Mz " << "E " << endl;
        std::vector<std::string> data(number_of_steps);

        //Run algorithm "number_of_steps" times and save energy and magnetisation for each step
        for (int j = 0; j < number_of_steps + 1; ++j){
            Vector3 magnetisation = measure::get_magnetization(lattice);
            flt energy = measure::get_energy(lattice, h, k, J);
            magnetisations[j] = magnetisation;
            flt mag = magnetisation.norm();
            flt mag_z = magnetisation(2);
            energies[j] = energy;

            adaptive_metropolis(lattice, temperature, J, Time, Ns, h, k);
            outFile2 << Ns*j << " " << mag << " " << mag_z << " "<< energy;
            if(j != number_of_steps) {outFile2 << std::endl;}
        }
        if(i + 1 != temperatures.size()){outFile2 << endl << "====" << endl;}
    }

    outFile2.close();   


}

// temperature
//F64 T = 2.0;
F64 J = 1.0;
F64 Time = 3600.0;
Spin h = Spin(0.0,0.0,0.0);
Spin k = Spin(0.0,0.0,0.0);
//Define maximal number of steps, step width and number of iterations 
int Nmax = 100000;
int Ns = 1000;
int number_of_steps = round(Nmax/Ns);

// lattice size
int L = 8;
int Lx = 8;
int Ly = 8;
int Lz = 8;
std::vector<double> temperatures = {0.5, 1.0};

int main() {
    simulation(temperatures, h, k, Lx, Ly, Lz, J, Nmax, Ns, Time);
}
