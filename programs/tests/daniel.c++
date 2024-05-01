#include <matplot/matplot.h>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Algorithm/Algorithm.h++>
#include <Metropolis/adaptive_metropolis.h++>

namespace plt = matplot;

const flt dt = 0.001;
// end Time
const flt t_end = 10.0;
// int Random Lattice Seed
const int seed = 69;
// side Lenth
const uint L = 16;

int main(int mainArgCount, char **mainArgs)
{

    Lattice lattice(L,L,L);
    uint i = 0;
    flt T = 0.0001;
    //for(flt T = 0.01; T <= 0.1; T+=0.01){
    cout << "running for T = " << T << endl << endl;
    omp_set_num_threads(1);
    rng::set_seed(seed);
    lattice.randomize();
    adaptive_metropolis(lattice, T, 1.0, _inf_, 10'000'000, Spin{0,0,0}, Spin{0,0,0});
    //metropolis(lattice, T, 1.0, Spin{0,0,0}, Spin{0,0,0}, _inf_, 100'000,MoveType::Addaptive);
    //wolff(lattice,T, 1.0, _inf_, 10000);
    data::plot_lattice(lattice);
}