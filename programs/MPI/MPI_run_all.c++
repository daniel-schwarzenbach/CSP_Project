#include <Wolff/wolff.h++>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <fstream>
#include <vector>
#include <string>
#include <Metropolis/metropolis.h++>
#include <mpi.h>

//string get_filename(Spin h, Spin k, int Lx, int Ly, int Lz, float J,)

void simulation(std::vector<double> temperatures, Spin h, Spin k, int Lx, int Ly, int Lz, float J, int64_t Nwolff, int64_t Nmet, int Ns, float Time){
    //      METROPOLIS
    const uint seed = 42;
    rng::set_seed(seed);
    int number_of_steps_met = round(Nmet/Ns);
    int number_of_steps_wolff = round(Nwolff/Ns);

    std::string filename_metro = "Metropolis__Lattice Size_" + to_string(Lx) + "x" + to_string(Ly) + "x" + to_str(Lz) + "__hz_" + to_str(h(2)) + "__kz_" + to_str(k(2)) + "_T_" ".txt"; //Create .txt file with name "mag_and_energy_T.txt"


    std::ofstream outFile(filename_metro);
    if (outFile.is_open()) {
        // Write output data to file
        outFile << filename_metro << endl << "Metropolis" << endl << "L = " <<  Lx << "," << Ly << "," << Lz << endl << "h = " << h(0)<< "," << h(1) << "," << h(2) << endl
        << "k = " << k(0)<< "," << k(1) << "," << k(2) << endl << "Ns = " << Ns << endl << "Nmax = " << Nmet << endl << "No_of_datapoints = " << number_of_steps_met << endl;
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
        Array<f64> energies(number_of_steps_met + 1); 

        double temperature = temperatures[i]; //Get temperature

        cout << "Simulating at temperature: " << temperature << endl;

        std::vector<std::vector<double>> newData;
        outFile << to_string(temperature) << " M " << "Mz " << "E " << endl;

        //Run algorithm "number_of_steps" times and save energy and magnetisation for each step
        for (int j = 0; j < number_of_steps_met + 1; ++j){
            Vector3 magnetisation = measure::get_magnetization(lattice);
            flt energy = measure::get_energy(lattice, h, J, k);
            magnetisations[j] = magnetisation;
            flt mag = magnetisation.norm();
            flt mag_z = magnetisation(2);
            energies[j] = energy;

            metropolis(lattice, temperature, J, Time, Ns, h, k, MoveType::SmallStep);
            outFile << Ns*j << " " << mag << " " << mag_z << " "<< energy;
            if(j != number_of_steps_met) {outFile << std::endl;}
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
        << "k = " << k(0)<< "," << k(1) << "," << k(2) << endl << "Ns = " << Ns << endl << "Nmax = " << Nwolff << endl << "No_of_datapoints = " << number_of_steps_wolff << endl;
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
        //Plot magnetisation and energy as a function of steps to determine N_eq (until equilibrium)
        Array<Vector3> magnetisations(number_of_steps_wolff + 1); 
        Array<f64> energies(number_of_steps_wolff + 1); 

        double temperature = temperatures[i]; //Get temperature

        cout << "Simulating at temperature: " << temperature << endl;

        outFile1 << to_string(temperature) << " M " << "Mz " << "E " << endl;
        std::vector<std::string> data(number_of_steps_wolff);

        //Run algorithm "number_of_steps" times and save energy and magnetisation for each step
        for (int j = 0; j < number_of_steps_wolff + 1; ++j){
            Vector3 magnetisation = measure::get_magnetization(lattice);
            flt energy = measure::get_energy(lattice, h, J, k);
            magnetisations[j] = magnetisation;
            flt mag = magnetisation.norm();
            flt mag_z = magnetisation(2);
            energies[j] = energy;

            wolff(lattice, temperature, J, Time, Ns);
            outFile1 << Ns*j << " " << mag << " " << mag_z << " "<< energy;
            if(j != number_of_steps_wolff) {outFile1 << std::endl;}
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
        << "k = " << k(0)<< "," << k(1) << "," << k(2) << endl << "Ns = " << Ns << endl << "Nmax = " << Nmet << endl << "No_of_datapoints = " << number_of_steps_met << endl;
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
        Array<Vector3> magnetisations(number_of_steps_met + 1); 
        Array<flt> energies(number_of_steps_met + 1); 

        double temperature = temperatures[i]; //Get temperature

        cout << "Simulating at temperature: " << temperature << endl;

        outFile2 << to_string(temperature) << " M " << "Mz " << "E " << endl;
        std::vector<std::string> data(number_of_steps_met);

        //Run algorithm "number_of_steps" times and save energy and magnetisation for each step
        for (int j = 0; j < number_of_steps_met + 1; ++j){
            Vector3 magnetisation = measure::get_magnetization(lattice);
            flt energy = measure::get_energy(lattice, h, J, k);
            magnetisations[j] = magnetisation;
            flt mag = magnetisation.norm();
            flt mag_z = magnetisation(2);
            energies[j] = energy;

            adaptive_metropolis(lattice, temperature, J, Time, Ns, h, k);
            outFile2 << Ns*j << " " << mag << " " << mag_z << " "<< energy;
            if(j != number_of_steps_met) {outFile2 << std::endl;}
        }
        if(i + 1 != temperatures.size()){outFile2 << endl << "====" << endl;}
    }

    outFile2.close();   


}

// Function to generate an array of indices for a given rank
Array<int> generateIndices(int arraySize, int rank, int numberOfRanks) {
    Array<int> indices;
    int splitSize = arraySize / numberOfRanks;
    int startIndex = rank * splitSize;
    int endIndex = (rank == numberOfRanks - 1) ? arraySize : (startIndex + splitSize);
    // Generate the indices for the current rank
    for (int i = startIndex; i < endIndex; ++i) {
        indices.push_back(i);
    }
    return indices;
}

Array<flt> split_array(const Array<flt>& vec, int rank, int size) {
    // Calculate the size of each split
    size_t splitSize = vec.size() / size;
    size_t startIndex = rank * splitSize;
    size_t endIndex = (rank == size - 1) ? vec.size() : (startIndex + splitSize);

    // Handle the case where the vector cannot be evenly split
    if (vec.size() % size != 0 && rank == size - 1) {
        endIndex = vec.size();
    }

    // Create a sub-vector for the current rank
    Array<flt> subVec(vec.begin() + startIndex, vec.begin() + endIndex);
    return subVec;
}

// temperature
//f64 T = 2.0;
flt constexpr J = 1.0;
flt constexpr Time = 10000000.0;
uint constexpr Seed = 42;

const Spin h = Spin{0.0,0.0,0.0};
const Spin k = Spin{0.0,0.0,0.0};

constexpr uint L = 8;
constexpr uint Lx = L;
constexpr uint Ly = L; 
constexpr uint Lz = L;

constexpr u64 Ns = 1e3;


constexpr u64 N_wolff = 1e+5;
constexpr u64 N_met = 1e+8;
// constexpr u64 N_wolff = 1e+9;
// constexpr u64 N_met = 1e+11;

Array<double> temperatures = 
{1.00000000e-05, 3.75958333e-03, 7.50916667e-03, 1.12587500e-02
, 1.50083333e-02, 1.87579167e-02, 2.25075000e-02, 2.62570833e-02
, 3.00066667e-02, 3.37562500e-02, 3.75058333e-02, 4.12554167e-02
, 4.50050000e-02, 4.87545833e-02, 5.25041667e-02, 5.62537500e-02
, 6.00033333e-02, 6.37529167e-02, 6.75025000e-02, 7.12520833e-02
, 7.50016667e-02, 7.87512500e-02, 8.25008333e-02, 8.62504167e-02
, 9.00000000e-02, 1.00000000e-02, 6.28571429e-02, 1.15714286e-01
, 1.68571429e-01, 2.21428571e-01, 2.74285714e-01, 3.27142857e-01
, 3.80000000e-01, 4.32857143e-01, 4.85714286e-01, 5.38571429e-01
, 5.91428571e-01, 6.44285714e-01, 6.97142857e-01, 7.50000000e-01
, 8.02857143e-01, 8.55714286e-01, 9.08571429e-01, 9.61428571e-01
, 1.01428571e+00, 1.06714286e+00, 1.12000000e+00, 1.17285714e+00
, 1.22571429e+00, 1.27857143e+00, 1.33142857e+00, 1.38428571e+00
, 1.43714286e+00, 1.49000000e+00, 1.54285714e+00, 1.59571429e+00
, 1.64857143e+00, 1.70142857e+00, 1.75428571e+00, 1.80714286e+00
, 1.86000000e+00, 1.91285714e+00, 1.96571429e+00, 2.01857143e+00
, 2.07142857e+00, 2.12428571e+00, 2.17714286e+00, 2.23000000e+00
, 2.28285714e+00, 2.33571429e+00, 2.38857143e+00, 2.44142857e+00
, 2.49428571e+00, 2.54714286e+00, 2.60000000e+00, 1.50000000e+01
, 5.60416667e+01, 9.70833333e+01, 1.38125000e+02, 1.79166667e+02
, 2.20208333e+02, 2.61250000e+02, 3.02291667e+02, 3.43333333e+02
, 3.84375000e+02, 4.25416667e+02, 4.66458333e+02, 5.07500000e+02
, 5.48541667e+02, 5.89583333e+02, 6.30625000e+02, 6.71666667e+02
, 7.12708333e+02, 7.53750000e+02, 7.94791667e+02, 8.35833333e+02
, 8.76875000e+02, 9.17916667e+02, 9.58958333e+02, 1.00000000e+03};

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank;
    MPI_Comm_rank(comm, &rank);
    int size;
    MPI_Comm_size(comm, &size);
    Array<flt> local_temperatures = split_array(temperatures, rank, size);
    cout << rank <<" is running "<< local_temperatures.size() 
         << " out of " << temperatures.size() <<endl;
    if (size == 1){
        cerr << ERROR << "no parallel execution" << endl;
        return 0;
    }

    simulation(local_temperatures, h, k, Lx, Ly, Lz, J, N_wolff, N_met, Ns, Time);
    MPI_Finalize();
    return 0;

}