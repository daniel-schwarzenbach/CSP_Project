/*
This programm benchmarks the parrallel implementation of metropolis on
MPI. It takes a temperature as floating point and wites out multiple
data files
*/
#include <Simulations/Simulation.h++>
#include <Data/DataHandler.h++>
#include <mpi.h>
#include <ctime>

// variables
flt constexpr J = 1.0;
const Spin h = Spin{0.0,0.0,0.0};
const Spin k = Spin{0.0,0.0,0.0};
// lattice size
constexpr uint L = 32;
constexpr uint Lx = L;
constexpr uint Ly = L; 
constexpr uint Lz = L;
constexpr Index Ls = {Lx,Ly,Lz};
// number of steps
constexpr u64 Nmax_met = 1e+9;
constexpr u64 Ns_met = 1e+6;
// critical temperature
constexpr flt T_critical = 1.45;

int main(int argc, char* argv[])
{
    // init MPI
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank = 0;
    MPI_Comm_rank(comm, &rank);
    int size = 1;
    MPI_Comm_size(comm, &size);
    // measure the duration of the whole program
    measure::Timer programTimer;
    // get a random seed for each node
    const u64 FullSeed = static_cast<unsigned int>(time(0)) * rank;
    const uint Seed =
        static_cast<unsigned int>(FullSeed & 0xff'ff'ff'ffUL);
    // read input
    flt T = -1;
    if (argc > 1) {
        try {
            // Convert the first argument to a float
            T = data::read_flt(argv[1]);
        } catch (const std::invalid_argument& e) {
            cerr << ERROR 
                 << "Invalid argument: please enter a valid "
                 <<"floating-point number."<< endl;
        } catch (const std::out_of_range& e) {
            cerr << ERROR <<"Out of range: the number is too large." 
                 << endl;
            return 0;
        }
    } else {
        cerr << ERROR << "to few arguments";
        return 0;
    } 
    // cout << T = <T>
    what_is(T);
    // activate Loading bar fore a single core
    bool loading_bar = false;
    if (size == 1){
        loading_bar = true;
    }

    //      --- set Filenames
    string foldername = "T_" + to_string(uint(T*1000)) + "e-3";
    data::make_folder(foldername);
    
    string metropolisFile = foldername +"/Metropolis_";
    string metropolisAdaptFile = foldername +"/MetropolisAdaptive_";
    string metropolisFile_omp = foldername +"/Metropolis_omp";
    string metropolisAdaptFile_omp = foldername 
            +"/MetropolisAdaptive_omp";

    //      --- init Lattice
    Lattice lattice(Lx,Ly,Lz);

    //         --- Metropolis
    {
        measure::Timer watch; watch.start();
        cout << rank <<" is running metropolis ..."<< endl;
        rng::set_seed(Seed);
        // set lattice after the critical temperature
        if(T > T_critical)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});
        // run algorithm
        Array2D<flt> data = 
                sim::ns::test_algorithm(lattice, Ns_met, Nmax_met, T,
                        J, h, k, sim::ns::metropolis_smallStep, 
                        loading_bar);
        // store collected data
        data::store_data(data,metropolisFile+to_string(rank));
        cout << "finished metropolis in: " << watch.time()
             << endl <<  endl;
    }



        //      --- Metropolis Adaptive
    {   
        measure::Timer watch; watch.start();
        cout << rank <<" is running metropolis adaptive ..."<< endl;
        rng::set_seed(Seed);
        // set lattice after the critical temperature
        if(T > T_critical)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});
        // run algorithm
        Array2D<flt> data = 
                sim::ns::test_algorithm(lattice, Ns_met, Nmax_met, T,
                        J, h, k, sim::ns::metropolis_adaptive,
                        loading_bar);
        // store collected data
        data::store_data(data,metropolisAdaptFile + to_string(rank));
        cout << "finished metropolis adaptive in: " << watch.time() 
             << endl << endl;
    }

        //         --- Metropolis
    {
        measure::Timer watch; watch.start();
        cout << rank <<" is running metropolis ..."<< endl;
        rng::set_seed(Seed);
        // set lattice after the critical temperature
        if(T > T_critical)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});
        // run algorithm
        Array2D<flt> data = 
                sim::ns::test_algorithm(lattice, Ns_met, Nmax_met, T,
                        J, h, k, sim::ns::metropolis_smallStep_omp, 
                        loading_bar);
        // store collected data
        data::store_data(data,metropolisFile_omp+to_string(rank));
        cout << "finished metropolis in: " 
             << watch.time() << endl << endl;
    }



        //      --- Metropolis Adaptive OMP
    {   
        measure::Timer watch; watch.start();
        cout << rank <<" is running metropolis adaptive omp ..."
                << endl;
        // set lattice after the critical temperature
        rng::set_seed(Seed);
        if(T > T_critical)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});
        // run algoritm
        Array2D<flt> data = 
                sim::ns::test_algorithm(lattice, Ns_met, Nmax_met, T,
                        J, h, k, sim::ns::metropolis_adaptive_omp,
                        loading_bar);
        // store the resulting data
        data::store_data(data,metropolisAdaptFile_omp 
                + to_string(rank));
        cout << "finished metropolis adaptive omp in: " 
             << watch.time() << endl << endl;
    }

            //         --- Metropolis OMP
    {
        measure::Timer watch; watch.start();
        cout << rank <<" is running metropolis ..."<< endl;
        rng::set_seed(Seed);
        if(T > 1.45)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});
        
        // run algortim
        Array2D<flt> data = 
                sim::ns::test_algorithm(lattice, Ns_met, Nmax_met, T,
                        J, h, k, sim::ns::metropolis_smallStep_omp, 
                        loading_bar);
        // store the resulting data
        data::store_data(data,metropolisFile_omp+to_string(rank));
        cout << "finished metropolis omp in: " 
             << watch.time() << endl << endl;
    }
    cout << "finished Program with: ";what_is(programTimer.time());
    MPI_Finalize();
    return 0;
}