#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Metropolis/metropolis.h++>
#include <Wolff/wolff.h++>
#include <Measure/LoadingBar.h++>
#include <format>

// int Random Lattice Seed
const int seed = 42;
// side Lenth
const uint L = 8;
// interaction strenth
const flt J = 1.0;
// set Temperature
const flt T = 0.01;


int main(int mainArgCount, char **mainArgs)
{
    // make folder to store the plots
    data::make_folder("plot");
    // init lattice
    Lattice3D<Spin> lattice(L, L, L);
    lattice.randomize();
    // plot after 50 steps
    const u64 Ns = 10;
    // final number of steps
    const u64 Nmax = 2000;
    uint it = 0; // to name files
    // start 
    cout << "start Ploting ..."  << endl;
    // init loading bar
    {
    measure::LoadingBar lbar(60);
    for (u64 i = 0; i < Nmax; i+=Ns)
    {
        // updade the loading bar
        lbar.update((i * 100.0) / Nmax);
        // do Ns steps
        metropolis_omp(lattice,T,J,_inf_,Ns,{0,0,0},{0,0,0},
                MoveType::Random);
        // plot the lattice
        std::string num = std::format("{:03}", it);
        data::plot_lattice_small(lattice,"plot/Lattice_" + num
                + ".png");
        ++it;
    }
    lbar.update(100);
    }
    cout << "finished ploting" << endl;
    // make a gif
    cout << "make gif ..."  << endl;
    data::convert_pngs_to_gif("plot/metropolis.gif", "plot/");
    cout << "finished" << endl;
    return 0;
}