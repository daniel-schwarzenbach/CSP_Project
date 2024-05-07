#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Metropolis/metropolis.h++>

// int Random Lattice Seed
const int seed = 42;
// side Lenth
const uint L = 8;
// interaction strenth
const flt J = 1.0;
// set Temperature
const flt T = 0.1;


int main(int mainArgCount, char **mainArgs)
{
    data::make_folder("plot");
    Lattice3D<Spin> lattice(L, L, L);
    lattice.randomize();
    
    const u64 Ns = 1'000;
    const u64 Nmax = 100'000;
    uint it = 0;
    for (u64 i = 0; i < Nmax; i+=Ns)
    {
        metropolis_omp(lattice,T,J,_inf_,Ns);
        data::plot_lattice(lattice,"plot/Lattice_" + to_string(it)
                + ".png");
        ++it;
    }
    return 0;
}