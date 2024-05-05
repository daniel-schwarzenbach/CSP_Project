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
#include <functional>

std::basic_ostream<char>& operator<<(std::basic_ostream<char>& of, std::vector<double> const& array){
    for (double x : array){
        of << x << " ";
    }
    return of;
    //of << "\n";
}

flt mean(const std::vector<double>& values, int start, int end) {
    flt sum = 0.0;
    int count = 0;
    for (int i = start; i < end; ++i) {
        sum += values[i];
        ++count;
    }
    if (count == 0) return 0.0; // To handle cases where start >= end
    return sum / count;
}

// Loop over specified system at different temperatures
// function defintion:
//  Inputs: array of temperatures, h, k, 
//  Output: void, prints results to file

void simulation(std::vector<double> temperatures, Spin h, Spin k, int Lx, int Ly, int Lz, float J,
    int64_t Nmax_met, int64_t Nmax_adamet, int64_t Nmax_wolff, int Ns_met, int Ns_adamet, int Ns_wolff, float Time){
    //      METROPOLIS
    const uint seed = 42;
    rng::set_seed(seed);
    int number_of_steps_met = round(Nmax_met/Ns_met);
    int number_of_steps_adamet = round(Nmax_adamet/Ns_adamet);
    int number_of_steps_wolff = round(Nmax_wolff/Ns_wolff);

    std::string filename_metro = "Metropolis, Lattice Size " + to_string(Lx) + "x" + to_string(Ly) + "x" + to_string(Lz) + ", h_z " + to_string(h(2)) + ", k_z " + to_string(k(2)) + "Steps " + to_string(Ns_met) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"


    std::ofstream outFile(filename_metro);
    if (outFile.is_open()) {
        // Write output data to file
        outFile << filename_metro << endl << "Metropolis" << endl << "L = " <<  Lx << "," << Ly << "," << Lz << endl << "h = " << h(0)<< "," << h(1) << "," << h(2) << endl
        << "k = " << k(0)<< "," << k(1) << "," << k(2) << endl << "Ns = " << Ns_met << endl << "Nmax = " << Nmax_met << endl << "No_of_datapoints = " << number_of_steps_met << endl;
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
        Array<Vector3> magnetisations(number_of_steps_met + 1); 
        Array<F64> energies(number_of_steps_met + 1); 

    //     double temperature = temperatures[i]; //Get temperature

    //     cout << "Simulating at temperature: " << temperature << endl;

    //     std::vector<std::vector<double>> newData;
    //     outFile << to_string(temperature) << " M " << "Mz " << "E " << endl;

        //Run algorithm "number_of_steps" times and save energy and magnetisation for each step
        for (int j = 0; j < number_of_steps_met + 1; ++j){
            Vector3 magnetisation = measure::get_magnetization(lattice);
            flt energy = measure::get_energy(lattice, h, k, J);
            magnetisations[j] = magnetisation;
            flt mag = magnetisation.norm();
            flt mag_z = magnetisation(2);
            energies[j] = energy;

            metropolis(lattice, temperature, J, Time, Ns_met, h, k, MoveType::SmallStep);
            outFile << Ns_met*j << " " << mag << " " << mag_z << " "<< energy;
            if(j != number_of_steps_met) {outFile << std::endl;}
            cout << Ns_met*j <<  " mag = " << mag << ", energy = " << energy << endl;
        }
        if(i + 1 != temperatures.size()){outFile << endl << "====" << endl;}
    }

    outFile.close();
/*
    
    //        WOLFF

    std::string filename_wolff = "Wolff, Lattice Size " + to_string(Lx) + "x" + to_string(Ly) + "x" + to_string(Lz) + ", h_z " + to_string(h(2)) + ", k_z " + to_string(k(2)) + "Steps " + to_string(Ns_wolff) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"


    std::ofstream outFile1(filename_wolff);
    if (outFile1.is_open()) {
        // Write output data to file
        outFile1 << filename_wolff << endl << "Wolff" << endl << "L = " <<  Lx << "," << Ly << "," << Lz << endl << "h = " << h(0)<< "," << h(1) << "," << h(2) << endl
        << "k = " << k(0)<< "," << k(1) << "," << k(2) << endl << "Ns = " << Ns_wolff << endl << "Nmax = " << Nmax_wolff << endl << "No_of_datapoints = " << number_of_steps_wolff << endl;
        for (size_t i = 0; i < temperatures.size(); ++i) {
            outFile1 << temperatures[i];
            if (i+1 != temperatures.size()){
                outFile1 << ",";
            }
        }
        outFile1 << endl << "====" << endl;
        // Write additional simulation results if needed
        cout << "Output saved to " << filename_wolff << endl;
    } 
    else {
        cout << "Error: Unable to open file " << filename_wolff << " for writing." << endl;
    }

    for (size_t i = 0; i < temperatures.size(); ++i) {
        Lattice lattice = Lattice::random_lattice(Lx, Ly, Lz);
        //Lattice lattice = Lattice::constant_lattice(Lx,Ly,Lz, Spin(1.0,0.0,0.0));
        //Plot magnetisation and energy as a function of steps to determine N_eq (until equilibrium)
        Array<Vector3> magnetisations(number_of_steps_wolff + 1); 
        Array<F64> energies(number_of_steps_wolff + 1); 

        double temperature = temperatures[i]; //Get temperature 

        cout << "Simulating at temperature: " << temperature << endl;

        outFile1 << to_string(temperature) << " M " << "Mz " << "E " << endl;
        std::vector<std::string> data(number_of_steps_wolff);

        //Run algorithm "number_of_steps" times and save energy and magnetisation for each step
        for (int j = 0; j < number_of_steps_wolff + 1; ++j){
            Vector3 magnetisation = measure::get_magnetization(lattice);
            flt energy = measure::get_energy(lattice, h, k, J);
            magnetisations[j] = magnetisation;
            flt mag = magnetisation.norm();
            flt mag_z = magnetisation(2);
            energies[j] = energy;

            wolff(lattice, temperature, J, Time, Ns_wolff);
            outFile1 << Ns_wolff*j << " " << mag << " " << mag_z << " "<< energy;
            if(j != number_of_steps_wolff) {outFile1 << std::endl;}
        }
        if(i + 1 != temperatures.size()){outFile1 << endl << "====" << endl;}
    }

    outFile1.close();

    /*

    //        ADAPTIVE METROPOLIs

    std::string filename_adametro = "Adaptive Metropolis, Lattice Size " + to_string(Lx) + "x" + to_string(Ly) + "x" + to_string(Lz) + ", h_z " + to_string(h(2)) + ", k_z " + to_string(k(2)) + "Steps " + to_string(Ns_adamet) + ".txt"; //Create .txt file with name "mag_and_energy_T.txt"


    std::ofstream outFile2(filename_adametro);
    if (outFile2.is_open()) {
        // Write output data to file
        outFile2 << filename_adametro << endl << "Adaptive Metropolis" << endl << "L = " <<  Lx << "," << Ly << "," << Lz << endl << "h = " << h(0)<< "," << h(1) << "," << h(2) << endl
        << "k = " << k(0)<< "," << k(1) << "," << k(2) << endl << "Ns = " << Ns_adamet << endl << "Nmax = " << Nmax_adamet << endl << "No_of_datapoints = " << number_of_steps_adamet << endl;
        for (size_t i = 0; i < temperatures.size(); ++i) {
            outFile2 << temperatures[i];
            if (i+1 != temperatures.size()){
                outFile2 << ",";
            }
        }
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
        Array<Vector3> magnetisations(number_of_steps_adamet + 1); 
        Array<F64> energies(number_of_steps_adamet + 1); 

        double temperature = temperatures[i]; //Get temperature

        cout << "Simulating at temperature: " << temperature << endl;

        outFile2 << to_string(temperature) << " M " << "Mz " << "E " << endl;

        //Run algorithm "number_of_steps" times and save energy and magnetisation for each step
        for (int j = 0; j < number_of_steps_adamet + 1; ++j){
            Vector3 magnetisation = measure::get_magnetization(lattice);
            flt energy = measure::get_energy(lattice, h, k, J);
            magnetisations[j] = magnetisation;
            flt mag = magnetisation.norm();
            flt mag_z = magnetisation(2);
            energies[j] = energy;

            adaptive_metropolis(lattice, temperature, J, Time, Ns_adamet, h, k);
            outFile2 << Ns_adamet*j << " " << mag << " " << mag_z << " "<< energy;
            if(j != number_of_steps_adamet) {outFile2 << std::endl;}
        }
        if(i + 1 != temperatures.size()){outFile2 << endl << "====" << endl;}
    }

    outFile2.close();   

*/
}

// Temperatures: temperatures at which we want to drive the system to
// equilibrium, array with N entries
// N_eq: number of steps needed for the different temperatures to bring
// the system to equilibrium, different for the three algorithms,
// array with dimension N x 3
void calc_ramp(std::vector<double> temperatures, std::vector<std::vector<double>> N_eq,
    int N_datapoints_met, int N_datapoints_adamet, int N_datapoints_wolff,
    Spin h, Spin k, int Lx, int Ly, int Lz, F64 J, F64 Time){
    // for loop over temperatures vector with index i
        // call function simulation with N_eq for the three different 
        // algorithms at temperatures[i] saved in N_eq[i, 0] for metropolis,
        // N_eq[i, 1] for adaptive metropolis, N_eq[i, 2] for Wolff.

    // for loop over temperatures vector with index i
    for (int i = 0; i < temperatures.size(); ++i) {
        // Access temperature at index i
        std::vector<double> current_temperature = {temperatures[i]};
        
        // Access N_eq values for current temperature
        int64_t Nmax_met_i = N_eq[0][i];
        int64_t Nmax_adamet_i = N_eq[1][i];
        int64_t  Nmax_wolff_i = N_eq[2][i];
        int Ns_met = round(Nmax_met_i/N_datapoints_met);
        int Ns_adamet = round(Nmax_adamet_i/N_datapoints_adamet);
        int Ns_wolff = round(Nmax_wolff_i/N_datapoints_wolff);
        
        simulation(current_temperature, h, k, Lx, Ly, Lz, J, Nmax_met_i, Nmax_adamet_i,
            Nmax_wolff_i, Ns_met, Ns_adamet, Ns_wolff, Time);
        // Process simulation result as needed
    }
}

void benchmark_algorithms(string algorithm_name, int N_eq, int M_max, 
    int N_s, int N_max, int L, float T, float J, float MaxTime, 
    Spin h, Spin k){
    // Check which algorithm is benchmarked
    if (algorithm_name == "Metropolis") {
        std::function<bool(Lattice&, F64, F64, F64, int64_t, Spin, Spin, MoveType)> algorithm = metropolis;
    } else if (algorithm_name == "Adaptive Metropolis") {
        std::function<bool(Lattice&, F64, F64, F64, int64_t, Spin, Spin, F64)> algorithm = adaptive_metropolis;
    } else if (algorithm_name == "Wolff") {
        std::function<bool(Lattice&, F64, F64, F64, u64)> algorithm = wolff;
    } else {
        std::cerr << "Error: Unknown algorithm\n";
        return;
    }
    std::cout << "Benchmarking " << algorithm_name << endl;
    // Perform benchmarking
    for (int i = 0; i < M_max; ++i) {
        uint seed = i;
        rng::set_seed(i);
        Lattice lattice = Lattice::random_lattice(L, L, L);

        // Check which algorithm is benchmarked
        if (algorithm_name == "Metropolis") {
            metropolis(lattice, T, J, MaxTime, N_eq, h, k);
        } else if (algorithm_name == "Adaptive Metropolis") {
            adaptive_metropolis(lattice, T, J, MaxTime, N_eq, h, k);
        } else if (algorithm_name == "Wolff") {
            wolff(lattice, T, J, MaxTime, N_eq);;
        } else {
            std::cerr << "Error: Unknown algorithm\n";
            return;
        }

        // Variables to track sum and sum of squares
        double energy_sum = 0.0;
        double energy_sum_squared = 0.0;
        double magnetization_sum = 0.0;
        double magnetization_sum_squared = 0.0;

        // Perform N_s steps N_max times
        for (int j = 0; j < N_max; ++j) {
            // calculate M and E and their variance
            flt energy = measure::get_energy(lattice, h, k, J);
            Vector3 magnetisation = measure::get_magnetization(lattice);
            // Now calculate variance up until now
            // Update sums and sum of squares
            energy_sum += energy;
            energy_sum_squared += energy * energy;
            magnetization_sum += magnetisation.norm();
            magnetization_sum_squared += magnetisation.norm() * magnetisation.norm();
            // Calculate variance up until now
            double energy_variance = (energy_sum_squared / (j + 1)) - std::pow(energy_sum / (j + 1), 2);
            double magnetization_variance = (magnetization_sum_squared / (j + 1)) - std::pow(magnetization_sum / (j + 1), 2);
            // Perform N_s steps of the algorithm
            // Check which algorithm is benchmarked
            if (algorithm_name == "Metropolis") {
                metropolis(lattice, T, J, MaxTime, N_s, h, k);
            } else if (algorithm_name == "Adaptive Metropolis") {
                adaptive_metropolis(lattice, T, J, MaxTime, N_s, h, k);
            } else if (algorithm_name == "Wolff") {
                wolff(lattice, T, J, MaxTime, N_s);;
            } else {
                std::cerr << "Error: Unknown algorithm\n";
                return;
            }
        }
    }
}

// temperature
//F64 T = 2.0;
F64 J_ = 1.0;
F64 Time_ = 100000.0;
Spin h_ = Spin(0.0,0.0,0.0);
Spin k_ = Spin(0.0,0.0,0.0);
//Define maximal number of steps, step width and number of iterations 
int64_t Nmax_met = 1e3;
int64_t Nmax_adamet = 1e9;
int64_t Nmax_wolff = 1e9;
int Ns_met = 20;
int Ns_adamet = 1e3;
int Ns_wolff = 1e3;

// lattice size
int L = 8;
int Lx_ = 8;
int Ly_ = 8;
int Lz_ = 8;
int N_datapoints_met = 10;
int N_datapoints_adamet = 10;

int N_datapoints_wolff = 10;
std::vector<double> temperatures = {0.01};

//std::vector<std::vector<double>> N_eq_ = {{169,307,930},{135,246,744},{15,26,74}};
//std::vector<double> temperatures_ = {0.01};
//{-2.6,-2.5,-2.4000000000000004,-2.1,-2.0,-1.9000000000000001}
//{{1000,885,771,428,314,200},{2007,1914,1821,1542,1450,1357},{302,294,285,260,251,242}}
std::vector<string> algos= {"Metropolis","Wolff","Adaptive Metropolis"};
// Define arrays for N_eq, M_max, N_s, N_max for each algorithm
std::vector<int> N_eq_values = {100, 200, 150};   // Example values, replace with appropriate values
std::vector<int> M_max_values = {10, 20, 15};     // Example values, replace with appropriate values
std::vector<int> N_s_values = {1000, 1000, 1000};       // Example values, replace with appropriate values
std::vector<int> N_max_values = {20, 30, 25};     // Example values, replace with appropriate values
float temp = 0.01;
float maxTime = 100;


int main()
{   
    TimeKeeper watch;
    simulation(temperatures, h_, k_, Lx_, Ly_, Lz_, J_, Nmax_met, Nmax_adamet,
    Nmax_wolff, Ns_met, Ns_adamet, Ns_wolff, Time_);
    std::cout << watch.time();
    //calc_ramp(temperatures_, N_eq_, N_datapoints_met, N_datapoints_adamet,
    //    N_datapoints_wolff, h_, k_, Lx_, Ly_, Lz_, J_, Time_);
    // Iterate over each algorithm name and benchmark it
    // Iterate over each algorithm name and benchmark it
    //for (size_t i = 0; i < algos.size(); ++i) {
    //    benchmark_algorithms(algos[i], N_eq_values[i], M_max_values[i], N_s_values[i], N_max_values[i], L, temp, J_, maxTime, h_, k_);
    //}
    return 0;
}
