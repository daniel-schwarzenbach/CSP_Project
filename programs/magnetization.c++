#include <matplot/matplot.h>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Simulations/Simulation.h++>

const flt Ns_met = 1e+1;
// end Time
const flt Nmax_met = 1e+4;

// int Random Lattice Seed
const int seed = 42;
// side Lenth
const uint L = 8;
// interaction strenth
const flt J = 1.0;
// h
const Spin h = {0, 0, 1};


int main(int mainArgCount, char **mainArgs)
{
    data::make_folder("data");
    Lattice3D<Spin> lattice(L, L, L);
    
    Array<flt> Ts = {0.5, 1.5, 3};
    for (flt T : Ts)
    {
        const Spin k = {0, 0, 0};
        cout << "running for T = " << T << endl;
        string Tstr = "T_" + to_string(uint(T * 1e3)) + "e-3/";
        string Folder = "data/" + Tstr;
        data::make_folder(Folder);


        cout << "running small step metropolis" << endl;

        rng::set_seed(seed);
        lattice.set_constant({0,0,-1});

        Array2D<flt> metro = sim::ns::test_algorithm(
            lattice, Ns_met, Nmax_met, T,
            J, h, k, sim::ns::metropolis_adaptive);
        data::store_data(metro,
                         Folder + "/metropolis_adaptive");
    }

    Array<flt> T2s = {0.5, 1.5, 3};
    for (flt T : T2s)
    {
        const Spin k = {0, 0, -1};
        cout << "running for T = " << T << endl;
        string Tstr = "T_" + to_string(uint(T * 1e3)) + "e-3/";
        string Folder = "data/" + Tstr;
        data::make_folder(Folder);


        cout << "running small step metropolis" << endl;

        rng::set_seed(seed);
        lattice.set_constant({0,0,-1});

        Array2D<flt> metro = sim::ns::test_algorithm(
            lattice, Ns_met, Nmax_met, T,
            J, h, k, sim::ns::metropolis_adaptive);
        data::store_data(metro,
                         Folder + "/metropolis_adaptive");
    }
    return 0;
}