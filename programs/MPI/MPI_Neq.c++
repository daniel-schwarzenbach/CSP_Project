#include <Algorithm/Algorithm.h++>
#include <Data/MPI_Helper.h++>
#include <Data/DataHandler.h++>
#include <mpi.h>
#include <ctime>   // for time()

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

constexpr flt T_critical = 1.45;
constexpr flt T_low = 1.3;
constexpr flt T_high = 1.6;


int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank = 0;
    MPI_Comm_rank(comm, &rank);
    int size = 1;
    MPI_Comm_size(comm, &size);
    measure::Timer programTimer; programTimer.start();
    // this is dumm and stupid!!!
    const u64 FullSeed = static_cast<unsigned int>(time(0))*rank;
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
    
    what_is(T);
    // activate Loading bar fore a single core
    bool loading_bar = false;
    if (size == 1){
        loading_bar = true;
    }

    //          --- init lattice
    Lattice lattice(Lx,Ly,Lz);

    //      --- set Filenames
    string foldername = "T_" + to_string(uint(T*1000)) + "e-3";
    data::make_folder(foldername);
    
    string metropolisFile = foldername + "/Metropolis_";
    string metropolisAdaptFile = foldername + "/MetropolisAdaptive_";
    string wolffFile = foldername + "/Wolff_";



    const u64 Nmax_met = 1e+8;
    const u64 Nmax_wolff = 1e6;

    const u64 Ns_met = 10'000;
    const u64 Ns_wolff = 100;



    //         --- Metropolis
    {
        measure::Timer watch; watch.start();
        cout << rank <<" is running metropolis ..."<< endl;
        string latticefile = ".././input/" + metropolisFile 
                + "Lattice_" + to_string(rank);
        what_is(latticefile);
        if(!data::load_lattice(lattice, latticefile)){
            cerr << "fatal error: coulndt read: " << latticefile;
            return 0;
        }
        
        cout << "T = " << T << endl;
        Array2D<flt> data = 
                algo::ds::test_algorithm(lattice, Ns_met, Nmax_met, T,
                        J, h, k, algo::ds::metropolis_smallStep, 
                        loading_bar);
        data::store_data(data,metropolisFile+to_string(rank));
        cout << "finished metropolis in: "<< watch.time()<<endl<<endl;
    }



//         //      --- Metropolis Adaptive
    {   
        measure::Timer watch; watch.start();
        string latticefile = ".././input/" + metropolisAdaptFile 
                + "Lattice_" + to_string(rank);
        what_is(latticefile);
        if(!data::load_lattice(lattice, latticefile)){
            cerr << "fatal error: coulndt read: " << latticefile;
            return 0;
        }
    
        cout << "T = " << T << endl;
        Array2D<flt> data = 
                algo::ds::test_algorithm(lattice, Ns_met, Nmax_met, T,
                        J, h, k, algo::ds::metropolis_adaptive,
                        loading_bar);
        data::store_data(data,metropolisAdaptFile + to_string(rank));
        cout<<"finished metropolis adaptive in: "
            <<watch.time()<<endl<<endl;
    }



//          --- Wolff
   {
        measure::Timer watch; watch.start();
        cout << rank <<" is running wolff ..."<< endl;
        string latticefile = ".././input/" + wolffFile 
                + "Lattice_" + to_string(rank);
        what_is(latticefile);
        if(!data::load_lattice(lattice, latticefile)){
            cerr << "fatal error: coulndt read: " << latticefile;
            return 0;
        }

        cout << "T = " << T << endl;
        Array2D<flt> data = 
                algo::ds::test_algorithm(lattice, Ns_wolff, Nmax_wolff, T,
                        J, h, k, algo::ds::wolff_, loading_bar);
        data::store_data(data,wolffFile + to_string(rank));
        cout << "finished wolff in: " << watch.time() << endl << endl;
    }
    cout << rank << " has finished the calculations" << endl;

    what_is(programTimer.time());
    MPI_Finalize();
    return 0;
}