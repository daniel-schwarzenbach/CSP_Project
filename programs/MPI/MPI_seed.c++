#include <Algorithm/Algorithm.h++>
#include <Data/MPI_Helper.h++>
#include <Data/DataHandler.h++>
#include <mpi.h>
#include <ctime>

flt constexpr J = 1.0;

const Spin h = Spin{0.0, 0.0, 0.0};
const Spin k = Spin{0.0, 0.0, 0.0};

constexpr uint L = 8;
constexpr uint Lx = L;
constexpr uint Ly = L;
constexpr uint Lz = L;
constexpr Index Ls = {Lx, Ly, Lz};
constexpr flt T_critical = 1.45;
constexpr flt T_low = 1.3;
constexpr flt T_high = 1.6;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank = 0;
    MPI_Comm_rank(comm, &rank);
    int size = 1;
    MPI_Comm_size(comm, &size);
    measure::Timer programTimer;
    programTimer.start();
    // set a ranndom seed
    const u64 FullSeed = static_cast<uint>(time(0)) * rank;
    const uint Seed =
        static_cast<unsigned int>(FullSeed & 0xff'ff'ff'ffUL);
    cout << " for rank " << rank << " has ";
    what_is(Seed);
    // read input
    flt T = -1;
    if (argc > 1)
    {
        try
        {
            // Convert the first argument to a float
            T = data::read_flt(argv[1]);
        }
        catch (const std::invalid_argument &e)
        {
            cerr << ERROR
                 << "Invalid argument: please enter a valid "
                 << "floating-point number." << endl;
        }
        catch (const std::out_of_range &e)
        {
            cerr << ERROR << "Out of range: the number is too large."
                 << endl;
            return 0;
        }
    }
    else
    {
        cerr << ERROR << "to few arguments";
        return 0;
    }
    // activate Loading bar fore a single core
    bool loading_bar = false;
    if (size == 1)
    {
        loading_bar = true;
    }

    //      --- set Filenames
    string foldername = "T_" + to_string(uint(T * 1000)) + "e-3";
    data::make_folder(foldername);

    string metropolisFile = foldername + "/Metropolis_";
    string metropolisAdaptFile = foldername + "/MetropolisAdaptive_";
    string wolffFile = foldername + "/Wolff_";

    u64 Nmax_met = 1e+9;
    if (T_low <= T && T_high >= T)
    {
        Nmax_met = 1e+10;
    }
    const u64 Nmax_wolff = 1e+8;

    const u64 Ns_met = ceil(flt(Nmax_met) / 1e3);
    const u64 Ns_wolff = ceil(flt(Nmax_wolff) / 1e3);

    //      --- init Lattice
    Lattice lattice(Lx, Ly, Lz);

    //         --- Metropolis
    {
        measure::Timer watch; watch.start();
        cout << rank <<" is running metropolis ..."<< endl;
        rng::set_seed(Seed);
        if(T > T_critical)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});

        cout << "T = " << T << endl;
        Array2D<flt> data =
                sim::ns::test_algorithm(lattice, Ns_met, Nmax_met, T,
                        J, h, k, sim::ns::metropolis_smallStep,
                        loading_bar);
        data::store_data(data,metropolisFile+to_string(rank));
        data::store_lattice(lattice,
                        metropolisFile+"Lattice_"+to_string(rank));
        cout << "finished metropolis in: "<<watch.time()<<endl<<endl;
    }

        //      --- Metropolis Adaptive
    {
        measure::Timer watch; watch.start();
        cout << rank <<" is running metropolis adaptive ..."<< endl;
        rng::set_seed(Seed);
        if(T > T_critical)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});

        cout << "T = " << T << endl;
        Array2D<flt> data =
                sim::ns::test_algorithm(lattice, Ns_met, Nmax_met, T,
                        J, h, k, sim::ns::metropolis_adaptive,
                        loading_bar);
        data::store_data(data,metropolisAdaptFile + to_string(rank));
        data::store_lattice(lattice,
                        metropolisAdaptFile+"Lattice_"
                        +to_string(rank));
        cout << "finished metropolis adaptive in: "
             << watch.time()<<endl<<endl;
    }

    //         --- Wolff
    {
        measure::Timer watch;
        watch.start();
        cout << rank << " is running wolff ..." << endl;
        rng::set_seed(Seed);
        if (T > T_critical)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0, 0, 1});

        cout << "T = " << T << endl;
        Array2D<flt> data =
            sim::ns::test_algorithm(lattice, Ns_wolff, Nmax_wolff, T,
                    J, h, k, sim::ns::wolff_, loading_bar);
        data::store_data(data, wolffFile + to_string(rank));
        data::store_lattice(lattice,
                            wolffFile + "Lattice_" + to_string(rank));
        cout << "finished wolff in: " << watch.time() << endl
             << endl;
    }
    cout << rank << " has finished the calculations" << endl;

    what_is(programTimer.time());
    MPI_Finalize();
    return 0;
}