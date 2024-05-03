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

void simulation(std::vector<double> temperatures, Spin h, Spin k, int Lx, int Ly, int Lz, float J, int64_t Nmax, int Ns, float Time){
    //      METROPOLIS
    const uint seed = 42;
    rng::set_seed(seed);
    int number_of_steps = round(Nmax/Ns);

    std::string filename_metro = "Metropolis, Lattice Size " + to_string(Lx) + "x" + to_string(Ly) + "x" + to_string(Lz) + ", h_z " + to_string(h(2)) + ", k_z " + to_string(k(2)) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"


    std::ofstream outFile(filename_metro);
    if (outFile.is_open()) {
        // Write output data to file
        outFile << filename_metro << endl << "Metropolis" << endl << "L = " <<  Lx << "," << Ly << "," << Lz << endl << "h = " << h(0)<< "," << h(1) << "," << h(2) << endl
        << "k = " << k(0)<< "," << k(1) << "," << k(2) << endl << "Ns = " << Ns << endl << "Nmax = " << Nmax << endl << "No_of_datapoints = " << number_of_steps << endl;
        // Loop over the vector with a counter
        for (size_t i = 0; i < temperatures.size(); ++i) {
            outFile << temperatures[i];
            if (i+1 != temperatures.size()){
                outFile << ",";
            }
        }
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

    std::string filename_wolff = "Wolff, Lattice Size " + to_string(Lx) + "x" + to_string(Ly) + "x" + to_string(Lz) + ", h_z " + to_string(h(2)) + ", k_z " + to_string(k(2)) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"


    std::ofstream outFile1(filename_wolff);
    if (outFile1.is_open()) {
        // Write output data to file
        outFile1 << filename_wolff << endl << "Wolff" << endl << "L = " <<  Lx << "," << Ly << "," << Lz << endl << "h = " << h(0)<< "," << h(1) << "," << h(2) << endl
        << "k = " << k(0)<< "," << k(1) << "," << k(2) << endl << "Ns = " << Ns << endl << "Nmax = " << Nmax << endl << "No_of_datapoints = " << number_of_steps << endl;
        for (size_t i = 0; i < temperatures.size(); ++i) {
            outFile1 << temperatures[i];
            if (i+1 != temperatures.size()){
                outFile1 << ",";
            }
        }
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

    std::string filename_adametro = "Adaptive Metropolis, Lattice Size " + to_string(Lx) + "x" + to_string(Ly) + "x" + to_string(Lz) + ", h_z " + to_string(h(2)) + ", k_z " + to_string(k(2)) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"


    std::ofstream outFile2(filename_adametro);
    if (outFile2.is_open()) {
        // Write output data to file
        outFile2 << filename_adametro<< endl << "Adaptive Metropolis" << endl << "L = " <<  Lx << "," << Ly << "," << Lz << endl << "h = " << h(0)<< "," << h(1) << "," << h(2) << endl
        << "k = " << k(0)<< "," << k(1) << "," << k(2) << endl << "Ns = " << Ns << endl << "Nmax = " << Nmax << endl << "No_of_datapoints = " << number_of_steps << endl;
        for (size_t i = 0; i < temperatures.size(); ++i) {
            outFile2 << temperatures[i];
            if (i+1 != temperatures.size()){
                outFile2 << ",";
            }
        }
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
F64 Time = 100000.0;
Spin h = Spin(0.0,0.0,0.0);
Spin k = Spin(0.0,0.0,0.0);
//Define maximal number of steps, step width and number of iterations 
int64_t Nmax = 1e7;
int Ns = 1e2;
int number_of_steps = round(Nmax/Ns);

// lattice size
int L = 8;
int Lx = 8;
int Ly = 8;
int Lz = 8;
std::vector<double> temperatures = {0.01};
/*
std::vector<double> temperatures = {
        1.00000000e-02, 5.06122449e-02, 9.12244898e-02, 1.31836735e-01,
        1.72448980e-01, 2.13061224e-01, 2.53673469e-01, 2.94285714e-01,
        3.34897959e-01, 3.75510204e-01, 4.16122449e-01, 4.56734694e-01,
        4.97346939e-01, 5.37959184e-01, 5.78571429e-01, 6.19183673e-01,
        6.59795918e-01, 7.00408163e-01, 7.41020408e-01, 7.81632653e-01,
        8.22244898e-01, 8.62857143e-01, 9.03469388e-01, 9.44081633e-01,
        9.84693878e-01, 1.02530612e+00, 1.06591837e+00, 1.10653061e+00,
        1.14714286e+00, 1.18775510e+00, 1.22836735e+00, 1.26897959e+00,
        1.30959184e+00, 1.35020408e+00, 1.39081633e+00, 1.43142857e+00,
        1.47204082e+00, 1.51265306e+00, 1.55326531e+00, 1.59387755e+00,
        1.63448980e+00, 1.67510204e+00, 1.71571429e+00, 1.75632653e+00,
        1.79693878e+00, 1.83755102e+00, 1.87816327e+00, 1.91877551e+00,
        1.95938776e+00, 2.00000000e+00, 1.00000000e-05, 3.75958333e-03,
        7.50916667e-03, 1.12587500e-02, 1.50083333e-02, 1.87579167e-02,
        2.25075000e-02, 2.62570833e-02, 3.00066667e-02, 3.37562500e-02,
        3.75058333e-02, 4.12554167e-02, 4.50050000e-02, 4.87545833e-02,
        5.25041667e-02, 5.62537500e-02, 6.00033333e-02, 6.37529167e-02,
        6.75025000e-02, 7.12520833e-02, 7.50016667e-02, 7.87512500e-02,
        8.25008333e-02, 8.62504167e-02, 9.00000000e-02, 1.50000000e+01,
        5.60416667e+01, 9.70833333e+01, 1.38125000e+02, 1.79166667e+02,
        2.20208333e+02, 2.61250000e+02, 3.02291667e+02, 3.43333333e+02,
        3.84375000e+02, 4.25416667e+02, 4.66458333e+02, 5.07500000e+02,
        5.48541667e+02, 5.89583333e+02, 6.30625000e+02, 6.71666667e+02,
        7.12708333e+02, 7.53750000e+02, 7.94791667e+02, 8.35833333e+02,
        8.76875000e+02, 9.17916667e+02, 9.58958333e+02, 1.00000000e+03
    };
*/


int main() {
    simulation(temperatures, h, k, Lx, Ly, Lz, J, Nmax, Ns, Time);
}
