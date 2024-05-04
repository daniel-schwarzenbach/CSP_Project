#include <Algorithm/Algorithm.h++>
#include <Data/MPI_Helper.h++>
#include <Data/DataHandler.h++>
#include <mpi.h>

flt constexpr J = 1.0;

const Spin h = Spin{0.0,0.0,0.0};
const Spin k = Spin{0.0,0.0,0.0};

constexpr uint L = 8;
constexpr uint Lx = L;
constexpr uint Ly = L; 
constexpr uint Lz = L;
constexpr Index Ls = {Lx,Ly,Lz};


constexpr u64 Ns_met = 1e+6;
constexpr u64 Ns_wolff = 1e+3;

constexpr u64 Nmax_met = 1e+9;
constexpr u64 Nmax_wolff = 1e+6;


int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank = 0;
    MPI_Comm_rank(comm, &rank);
    int size = 1;
    MPI_Comm_size(comm, &size);

    const uint Seed = 42 + rank;
    // read input
    flt T = -1;
    if (argc > 1) {
        try {
            // Convert the first argument to a float
            T = data::read_flt(argv[1]);
            cout << INFO << "T has been set to " << T;
        } catch (const std::invalid_argument& e) {
            cerr << ERROR 
                 << "Invalid argument: please enter a valid "
                 <<"floating-point number."<< endl;
        } catch (const std::out_of_range& e) {
            cerr << "Out of range: the number is too large." << endl;
        }
    } else {
        cerr << "No floating-point number was provided as an argument." 
            << endl;
    } 
    // set T to 1
    if(T < 0){
        cerr << ERROR << "no imput could been read!" << endl
             << "T = 1.0 by default" << endl;
        T = 1;
    }
    cout << rank <<" is running for T = "<<  T
         << " out of " << size << " ranks" << endl;
    // activate Loading bar fore a single core
    bool loading_bar = false;
    if (size == 1){
        loading_bar = true;
    }


    //      --- set Filenames
    string foldername = "T_" + to_string(uint(T*1000)) + "e3";
    data::make_folder(foldername);
    
    string metropolisFile = foldername + "/Metropolis_";
    string metropolisAdaptFile = foldername + "/MetropolisAdaptive_";
    string wolffFile = foldername + "/Wolff_";
    //      --- init Lattice
    Lattice lattice(Lx,Ly,Lz);



    //      --- Metropolis
    {
        cout << rank <<" is running metropolis ..."<< endl;
        rng::set_seed(Seed);
        if(T > 1.3)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});
        
        cout << "T = " << T << endl;
        Array2D<flt> data = 
                algo::ds::test_algorithm(lattice, Ns_met, Nmax_met, T,
                        J, h, k, algo::ds::metropolis_smallStep, 
                        loading_bar);
        data::store_data(data,metropolisFile+to_string(rank));
    }



        //      --- Metropolis Adaptive
    {   
        cout << rank <<" is running metropolis adaptive ..."<< endl;
        rng::set_seed(Seed);
        if(T > 1.3)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});

    
        cout << "T = " << T << endl;
        Array2D<flt> data = 
                algo::ds::test_algorithm(lattice, Ns_met, Nmax_met, T,
                        J, h, k, algo::ds::metropolis_adaptive,
                        loading_bar);
        data::store_data(data,metropolisAdaptFile + to_string(rank));
    }



    //      --- Wolff
    // run through all teperatures
   {
        cout << rank <<" is running wolff ..."<< endl;
        rng::set_seed(Seed);
        if(T > 1.3)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});

    
        cout << "T = " << T << endl;
        Array2D<flt> data = 
                algo::ds::test_algorithm(lattice, Ns_wolff, Nmax_wolff, T,
                        J, h, k, algo::ds::wolff_, loading_bar);
        data::store_data(data,wolffFile + to_string(rank));
    }
    MPI_Finalize();
    return 0;
}