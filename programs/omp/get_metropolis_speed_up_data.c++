/*
This program compares the parallel metorpolis adaptive with the non
parallelized version, in terms of physical quantaties and speed-up
*/
#include <matplot/matplot.h>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Simulations/Simulation.h++>

namespace plt = matplot;

const flt Ns_met = 5e+7;
// end Time
const flt Nmax_met = 5e+8;
// int Random Lattice Seed
const int seed = 124;
// interaction strenth
const flt J = 1.0;
// no magnetic field
const Spin h = {0, 0, 0};
// no anisotropy
const Spin k = {0, 0, 0};

int main(int mainArgCount, char **mainArgs)
{
    // temperatures
    Array<flt> Ts =
        {0.01, 0.1, 0.3, 0.5, 0.7, 1.0, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 
         2.0,
         3.0, 4.0, 5.0, 10, 100};
    Array<uint> Ls = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    u64 rows = Ts.size() * Ls.size();
    Array2D<flt> data(3);
    for (Array<flt> d : data)
    {
        d.reserve(rows);
    }
    // loop over every lattice size
    for (uint L : Ls)
    {
        what_is(L);
        // init lattice
        Lattice lattice(L, L, L);
        // loop over every temperature
        for (flt T : Ts)
        {
            what_is(T);

            //              --- adaptive step metropolis omp
            cout << "running metropolis omp" << endl;
            rng::set_seed(seed);
            // randomize lattice
            lattice.randomize();
            // reset adaptive variables
            restet_adaptive_omp();
            // test the algorithm
            Array2D<flt> metro_omp = sim::ns::test_algorithm(
                lattice, Ns_met, Nmax_met, T,
                J, h, k, sim::ns::metropolis_adaptive_omp);

            //              --- metropolis
            cout << "running metropolis" << endl;
            rng::set_seed(seed);
            // randomize lattice
            lattice.randomize();
            // reset adaptive variables
            restet_adaptive();
            // test the algorithm
            Array2D<flt> metro = sim::ns::test_algorithm(
                lattice, Ns_met, Nmax_met, T,
                J, h, k, sim::ns::metropolis_adaptive);

            // calculate and cout the parallel speed-up
            flt Speed_up = metro[4][metro[4].size() - 1] /
                           metro_omp[4][metro_omp[4].size() - 1];
            what_is(Speed_up);
            data[0].push_back(L);
            data[1].push_back(T);
            data[2].push_back(Speed_up);
            cout << endl;
        }
        data::store_data(data, "speed_up_data.txt");
    }
    return 0;
}