/*
This program callculates what happens if we add a magnetic field h and
and anisotropy k to a magnetiesed lattice

1. lattice = -z, h = +z, k = 0
2. lattice = -z, h = +z, k = -z
*/
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Simulations/Simulation.h++>

// number of steps between measuring
const flt Ns_met = 1e+4;
// number of steps
const flt Nmax_met = 1e+8;

// seed to set
const int seed = 42;
// side Lenth of the Lattice
const uint L = 8;
// interaction strenth
const flt J = 1.0;
// magnetic field
const Spin h = {0, 0, 1};


int main(int mainArgCount, char **mainArgs)
{
    // make a folder to store the data in
    data::make_folder("data");
    // init lattice
    Lattice3D<Spin> lattice(L, L, L);
    // do loop over Temperatures
    Array<flt> Ts = {0.5, 1.5, 3};

    //          ---- run with anisotropy k = 0
    for (flt T : Ts)
    {
        // set k = 0
        const Spin k = {0, 0, 0};
        cout << "running for T = " << T << endl;
        // define names
        string Tstr = "T=" + to_string(uint(T * 1e3)) + "e-3";
        string Folder = "data/k=0/";
        // make folder for k = 0
        data::make_folder(Folder);
        cout << "running metropolis" << endl;
        // set seed
        rng::set_seed(seed);
        // set lattice to -z
        lattice.set_constant({0,0,-1});
        // run algorithm
        Array2D<flt> metro = sim::ns::test_algorithm(
            lattice, Ns_met, Nmax_met, T,
            J, h, k, sim::ns::metropolis_smallStep);
        // store the resulting data
        data::store_data(metro,
                         Folder + "metropolis_small_step_" + Tstr);
    }

    //          ---- run with anisotropy k = -z
    for (flt T : Ts)
    {
        // set k = -z
        const Spin k = {0, 0, -1};
        cout << "running for T = " << T << endl;
        // define names
        string Tstr = "T" + to_string(uint(T * 1e3)) + "e-3";
        string Folder = "data/k=-z/";
        // make folder for k = -z
        data::make_folder(Folder);
        cout << "running metropolis" << endl;
        // set seed
        rng::set_seed(seed);
        // set lattice to -z
        lattice.set_constant({0,0,-1});
        // run algorithm
        Array2D<flt> metro = sim::ns::test_algorithm(
            lattice, Ns_met, Nmax_met, T,
            J, h, k, sim::ns::metropolis_smallStep);
        // store the resulting data
        data::store_data(metro,
                         Folder + "metropolis_small_step_" + Tstr);
    }
    return 0;
}