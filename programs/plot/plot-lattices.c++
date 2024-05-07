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
    
    const u64 Ns = 100;
    const u64 Nmax = 10'000;
    uint it = 0;
    for (u64 i = 0; i < Nmax; i+=Ns)
    {
        metropolis_omp(lattice,T,J,_inf_,Ns);
        data::plot_lattice(lattice,"plot/Lattice_" + to_string(0)
                + ".png");
    }
    return 0;
}