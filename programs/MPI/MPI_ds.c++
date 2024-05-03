#include <Algorithm/Algorithm.h++>
#include <Data/MPI_Helper.h++>
#include <Data/DataHandler.h++>
#include <mpi.h>

flt constexpr J = 1.0;
flt constexpr Time = 10000000.0;
uint constexpr Seed = 42;

const Spin h = Spin{0.0,0.0,0.0};
const Spin k = Spin{0.0,0.0,0.0};

constexpr uint L = 8;
constexpr uint Lx = L;
constexpr uint Ly = L; 
constexpr uint Lz = L;
constexpr Index Ls = {Lx,Ly,Lz};


constexpr u64 Ns = 1e3;
constexpr u64 N_wolff = 1e+6;
constexpr u64 N_met = 1e+9;
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

string metropolisSmallFile;
string metropolisAdaptFile;
string wolffFile;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank;
    MPI_Comm_rank(comm, &rank);
    int size;
    MPI_Comm_size(comm, &size);
    Array<flt> local_temperatures 
            = data::split_array(temperatures, rank, size);
    uint local_size = local_temperatures.size();
    cout << rank <<" is running "<< local_temperatures.size() 
         << " out of " << temperatures.size() <<endl;
    // make sure it runs in parallel
    // if (size == 1){
    //     cerr << ERROR << "no parallel execution" << endl;
    //     return 0;
    // }


    //      --- set Filenames
    metropolisSmallFile = "Metropolis_" + to_string(rank);
    metropolisAdaptFile = "MetropolisAdaptive_" + to_string(rank);
    wolffFile = "Wolff_" + to_string(rank);
    //      --- init Lattice
    Lattice lattice(Lx,Ly,Lz);



    //      --- Metropolis
    // run through all teperatures
    for(uint i = 0; i < local_size; ++i){
        rng::set_seed(Seed);
        lattice.randomize();
        const flt T = local_temperatures[i];
        cout << "T = " << T << endl;
        Array2D<flt> data = 
                algo::ds::test_algorithm(lattice, Ns, N_met, T,
                        J, h, k, algo::ds::metropolis_smallStep);
        if(i == 1){
            data::store_alo_data(metropolisSmallFile, "metropolis",
                                data, T, J, h, k, Ns, N_met, Ls);
        } else{
            data::append_algo_data(metropolisSmallFile, data, T);
        }
    }



        //      --- Metropolis Adaptive
    // run through all teperatures
    for(uint i = 0; i < local_size; ++i){
        rng::set_seed(Seed);
        lattice.randomize();
        const flt T = local_temperatures[i];
        cout << "T = " << T << endl;
        Array2D<flt> data = 
                algo::ds::test_algorithm(lattice, Ns, N_met, T,
                        J, h, k, algo::ds::metropolis_adaptive);
        if(i == 1){
            data::store_alo_data(metropolisAdaptFile, "adaptive metropolis",
                                data, T, J, h, k, Ns, N_met, Ls);
        } else{
            data::append_algo_data(metropolisAdaptFile, data, T);
        }
    }



        //      --- Wolff
    // run through all teperatures
    for(uint i = 0; i < local_size; ++i){
        rng::set_seed(Seed);
        lattice.randomize();
        const flt T = local_temperatures[i];
        cout << "T = " << T << endl;
        Array2D<flt> data = 
                algo::ds::test_algorithm(lattice, Ns, N_met, T,
                        J, h, k, algo::ds::wolff_);
        if(i == 1){
            data::store_alo_data(wolffFile, "wolff",
                                data, T, J, h, k, Ns, N_met, Ls);
        } else{
            data::append_algo_data(wolffFile, data, T);
        }
    }

    return 0;
}