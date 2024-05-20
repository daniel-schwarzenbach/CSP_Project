#include <Simulations/Simulation.h++>
#include <Data/DataHandler.h++>
#include <mpi.h>
#include <ctime>

// runs adaptive metropolis and gets sigma and aceptance rate
// additionally to steps, M, E, Time, Sigma
Array2D<flt> adaptive_sim(
    Lattice3D<Spin> &lattice,
    uint const &ns, u64 const &numSteps, flt const &T,
    flt const &J,
    Spin const &h, Spin const &k,
    bool const &loadingBar,
    u64 const& startStep,
    flt const& startTime)
{
    // get z Vector for measuring Mz
    Vector3 z = {0, 0, 1};
    // get max number of runs for reserving vectors
    uint maxSize = ceil(flt(numSteps) / flt(ns));

    // init data arrays, reserve to avoid time to reallocate data
    Array<flt> Time(0);
    Time.reserve(0);
    Array<flt> Step(0);
    Step.reserve(maxSize);
    Array<flt> M(0);
    M.reserve(maxSize);
    Array<flt> E(0);
    E.reserve(maxSize);
    Array<flt> Sigma(0);
    Step.reserve(maxSize);
    Array<flt> R(0);
    Step.reserve(maxSize);

    // init loding bar
    measure::LoadingBar lbar(60);
    // measure steps and elapsed time
    u64 step = startStep;
    flt t_elapsed = startTime;
    const u64 maxSteps = numSteps + startStep;
    while (step < maxSteps)
    {
        // update loading bar if requested
        if (loadingBar)
            lbar.update(flt(step) / flt(numSteps) * 100.0);
        // calculate data
        Step.push_back(step);
        Time.push_back(t_elapsed);
        Sigma.push_back(get_sigma_omp());
        R.push_back(get_acceptance_rate_omp());
        Vector3 magnVec = measure::get_magnetization(lattice);
        M.push_back(magnVec.norm());
        E.push_back(measure::get_energy(lattice));
        // track performance
        measure::Timer timer;
        timer.start();
        // run algorithm
        metropolis_omp(lattice, T, J, _inf_, ns, h, k, 
                MoveType::Addaptive);
        timer.stop();
        // update time
        t_elapsed += timer.time();
        // update number of steps
        step += ns;
    }
    if (loadingBar)
        lbar.update(100);
    Step.push_back(step);
    Time.push_back(t_elapsed);
    Sigma.push_back(get_sigma());
    R.push_back(get_acceptance_rate());
    Vector3 magnVec = measure::get_magnetization(lattice);
    M.push_back(magnVec.norm());
    E.push_back(measure::get_energy(lattice));

    return {Step, M, E, Time, Sigma, R};
}

void remove_Mz(Array2D<flt>& dataArray){
    dataArray.erase(dataArray.begin() + 2);
}

//      --- set constants

// interaction strenth
flt constexpr J = 1.0;
// magnetic field
const Spin h = Spin{0.0, 0.0, 0.0};
// anisotropy
const Spin k = Spin{0.0, 0.0, 0.0};
// lattice dimesions
constexpr uint L = 4;
constexpr uint Lx = L;
constexpr uint Ly = L;
constexpr uint Lz = L;
constexpr Index Ls = {Lx, Ly, Lz};
// temperature data
constexpr flt T_critical = 1.45;
constexpr flt T_low = 1.3;
constexpr flt T_high = 1.7;

int main(int argc, char *argv[])
{
    // init MPI
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank = 0;
    MPI_Comm_rank(comm, &rank);
    int size = 1;
    MPI_Comm_size(comm, &size);

    // measure program time
    measure::Timer programTimer;
    programTimer.start();

    // set a ranndom seed
    const u64 FullSeed = static_cast<uint>(time(0)) * (1+rank);
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

    // get N
    using NRange = StaticArray<u64,2>;
    StaticArray< NRange, 3> N_wolff;
    StaticArray< NRange, 3> N_met;
    StaticArray< NRange, 4> N_add;

    // low temperature
if (T < T_low)
{
    N_met = {NRange{u64(1e6), u64(1e2)}, NRange{u64(1e8),u64(1e4)}, 
            NRange{u64(1e7), u64(1e3)}};
    N_wolff = {NRange{u64(1e1), u64(1e0)}, NRange{u64(1e3),u64(1e0)}, 
            NRange{u64(1e5), u64(1e1)}};
    N_add = {NRange{u64(1e3), u64(1e0)}, NRange{u64(1e6),u64(1e2)}, 
            NRange{u64(1e8), u64(1e4)}, NRange{u64(1e7), u64(1e3)}};
}
// mid temperatures
else if(T <= T_high){
    N_met = {NRange{u64(1e7), u64(1e4)}, NRange{u64(1e9),u64(1e5)}, 
            NRange{u64(1e7), u64(1e3)}};
    N_wolff = {NRange{u64(1e1), u64(1e0)}, NRange{u64(1e5),u64(1e1)}, 
            NRange{u64(2e5), u64(1e1)}};
    N_add = {NRange{u64(1e3), u64(1e0)}, NRange{u64(1e6),u64(1e2)}, 
            NRange{u64(1e9), u64(1e5)}, NRange{u64(1e7), u64(1e3)}};
}
// high temperatures
else {
    N_met = {NRange{u64(1e6), u64(1e2)}, NRange{u64(1e8),u64(1e4)}, 
            NRange{u64(1e7), u64(1e3)}};
    N_wolff = {NRange{u64(1e1), u64(1e0)}, 
            NRange{u64(1e3),u64(1e0)}, 
            NRange{u64(1e6), u64(1e2)}};
    N_add = {NRange{u64(1e3), u64(1e0)}, NRange{u64(1e6),u64(1e2)}, 
            NRange{u64(1e8), u64(1e4)}, NRange{u64(1e7), u64(1e3)}};
}

    //      --- init Lattice
    Lattice lattice(Lx, Ly, Lz);


    //         --- Metropolis
    {
        measure::Timer watch;
        cout << rank <<" is running metropolis ..."<< endl;
        rng::set_seed(Seed);
        // set the lattice after the critical temperature
        if(T > T_critical)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});

        // run algorithm
        Array2D<flt> data = sim::ns::test_algorithm(lattice,
                        N_met[0][1], 
                        N_met[0][0], T,
                        J, h, k, sim::ns::metropolis_smallStep_omp,
                        loading_bar, 0);
        remove_Mz(data);
        {
        Array2D<flt> dataNew = sim::ns::test_algorithm(lattice,
                        N_met[1][1], 
                        N_met[1][0], T,
                        J, h, k, sim::ns::metropolis_smallStep_omp,
                        loading_bar,
                        data[0][data[0].size()-1],
                        data[3][data[3].size()-1]);
        remove_Mz(dataNew);
        push_back(data,dataNew);
        }
        {
        Array2D<flt> dataNew = sim::ns::test_algorithm(lattice,
                        N_met[2][1], 
                        N_met[2][0], T,
                        J, h, k, sim::ns::metropolis_smallStep_omp,
                        loading_bar,
                        data[0][data[0].size()-1],
                        data[3][data[3].size()-1]);
        remove_Mz(dataNew);
        push_back(data,dataNew);
        }
        data::store_data(data,metropolisFile+to_string(rank));
        cout << "finished metropolis in: "<<watch.time()<<endl<<endl;
    }

        //      --- Metropolis Adaptive
    {
        measure::Timer watch;
        cout << rank <<" is running adaptive ..."<< endl;
        rng::set_seed(Seed);
        // set the lattice after the critical temperature
        if(T > T_critical)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});

        // run algorithm
        Array2D<flt> data = adaptive_sim(lattice,
                        N_add[0][1], 
                        N_add[0][0], T,
                        J, h, k,
                        loading_bar, 0,0);
        {
        Array2D<flt> dataNew = adaptive_sim(lattice,
                        N_add[1][1], 
                        N_add[1][0], T,
                        J, h, k,
                        loading_bar,
                        data[0][data[0].size()-1],
                        data[3][data[3].size()-1]);
        push_back(data,dataNew);
        }
        {
        Array2D<flt> dataNew = adaptive_sim(lattice,
                        N_add[2][1], 
                        N_add[2][0], T,
                        J, h, k,
                        loading_bar ,
                        data[0][data[0].size()-1],
                        data[3][data[3].size()-1]);
        push_back(data,dataNew);
        }
        {
        Array2D<flt> dataNew = adaptive_sim(lattice,
                        N_add[3][1], 
                        N_add[3][0], T,
                        J, h, k,
                        loading_bar ,
                        data[0][data[0].size()-1],
                        data[3][data[3].size()-1]);
        push_back(data,dataNew);
        }
        data::store_data(data,metropolisAdaptFile+to_string(rank));
        cout << "finished adaptive metropolis in: "
             <<watch.time()  <<  endl <<  endl;
    }

    //         --- Wolff
    {
        
        measure::Timer watch;
        cout << rank <<" is running wolff ..."<< endl;
        rng::set_seed(Seed);
        // set the lattice after the critical temperature
        if(T > T_critical)
            lattice.randomize();
        else
            lattice.set_constant(Spin{0,0,1});

        // run algorithm
        Array2D<flt> data = sim::ns::test_algorithm(lattice,
                        N_wolff[0][1], 
                        N_wolff[0][0], T,
                        J, h, k, sim::ns::wolff_,
                        loading_bar);
        remove_Mz(data);
        {
        Array2D<flt> dataNew = sim::ns::test_algorithm(lattice,
                        N_wolff[1][1], 
                        N_wolff[1][0], T,
                        J, h, k, sim::ns::wolff_,
                        loading_bar ,
                        data[0][data[0].size()-1],
                        data[3][data[3].size()-1]);
        remove_Mz(dataNew);
        push_back(data,dataNew);
        }
        {
        Array2D<flt> dataNew = sim::ns::test_algorithm(lattice,
                        N_wolff[2][1], 
                        N_wolff[2][0], T,
                        J, h, k, sim::ns::wolff_,
                        loading_bar ,
                        data[0][data[0].size()-1],
                        data[3][data[3].size()-1]);
        remove_Mz(dataNew);
        push_back(data,dataNew);
        }
        data::store_data(data,wolffFile+to_string(rank));
        cout << "finished wolff in: "<< watch.time() << endl << endl;
    }


    cout << rank << " has finished the calculations" << endl;
    what_is(programTimer.time());
    MPI_Finalize();
    return 0;
}