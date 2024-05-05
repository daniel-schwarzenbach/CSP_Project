#include <matplot/matplot.h>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Algorithm/Algorithm.h++>

namespace plt = matplot;

const flt dt = 0.001;
// end Time
const flt t_end = 10.0;
// int Random Lattice Seed
const int seed = 69;
// side Lenth
const uint L = 2;

int main(int mainArgCount, char **mainArgs)
{

    Lattice lattice(L,L,L);
    uint i = 0;
    flt T = 1;
    flt J = 1;
    Spin h = {0,0,0};
    Spin k = {0,0,0};
    rng::set_seed(seed);
    lattice.randomize();
    data::plot_lattice(lattice, "random1.png");

    wolff(lattice, T, J, _inf_, 1, h);
    wolff(lattice, T, J, _inf_, 1, h);
    data::plot_lattice(lattice, "wolff1.png");

    rng::set_seed(seed);
    lattice.randomize();
    data::plot_lattice(lattice, "random2.png");
    wolff(lattice, T, J, _inf_, 2, h);
    data::plot_lattice(lattice, "wolff2.png");

    return 0;
}