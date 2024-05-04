#include <Algorithm/Algorithm.h++>
#include <Data/MPI_Helper.h++>
#include <Data/DataHandler.h++>
#include <mpi.h>

u64 get_wolf_steps(flt const& T){
    flt T_lower = 1.3;
    flt T_upper = 1.6;
    flt N_t0 =  1e4;
    flt N_tinf = 1e+6;
    flt T_inf = 100;
    flt T_0 = 0.01;
    flt N_tc = 1e+8;

    if (T <= T_lower)
        return u64( (N_t0 + (T - T_0) * (N_tc - N_t0))/(T_lower - T_0) );
    else if (T >= T_upper)
        return u64( (N_tc + (T_upper - T) * (N_tc - N_tinf))/(T_inf - T_upper) );
    else if (T_lower <= T <= T_upper)
        return u64( N_tc );
    else{
        cerr << "invalid temperature" << endl;
        return 1e-4;
    }
}

flt constexpr J = 1.0;

const Spin h = Spin{0.0,0.0,0.0};
const Spin k = Spin{0.0,0.0,0.0};

constexpr uint L = 8;
constexpr uint Lx = L;
constexpr uint Ly = L; 
constexpr uint Lz = L;
constexpr Index Ls = {Lx,Ly,Lz};





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
            cout << INFO << "T has been set to " << T << endl;
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

    

    constexpr u64 Nmax_met = 1e+9;
    const u64 Nmax_wolff = std::max(get_wolf_steps(T), 10'000UL);

    constexpr u64 Ns_met = 1e+6;
    const u64 Ns_wolff = ceil(flt(Nmax_wolff) / 1e-3);

    //      --- init Lattice
    Lattice lattice(Lx,Ly,Lz);



    //      --- Metropolis
    {
        measure::Timer watch; watch.start();
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
        cout << "finished metropolis in: " << watch.time() <<endl << endl;
    }



        //      --- Metropolis Adaptive
    {   
        measure::Timer watch; watch.start();
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
        cout << "finished metropolis adaptive in: " << watch.time() <<endl << endl;
    }



    //      --- Wolff
    // run through all teperatures
   {
        measure::Timer watch; watch.start();
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
        cout << "finished wolff in: " << watch.time() << endl << endl;
    }
    cout << rank << " has finished the calculations" << endl;
    MPI_Finalize();
    return 0;
}