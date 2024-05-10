/*
This program runs through the random step metropolis and plots the
lattice all Ns steps until Nmax. It than will make an animation out 
of it with ImageMagic

Linux:
> sudo apt-get install imagemagick

Mac-Os:
> brew install imagemagick
*/
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Metropolis/metropolis.h++>
#include <Wolff/wolff.h++>
#include <Measure/LoadingBar.h++>

// int Random Lattice Seed
const int seed = 42;
// side Lenth
const uint L = 8;
// interaction strenth
const flt J = 1.0;
// set Temperature
const flt T = 0.01;
// plot after 50 steps
const u64 Ns = 500;
// final number of steps
const u64 Nmax = 100'000;


int main(int mainArgCount, char **mainArgs)
{
    // make folder to store the plots
    data::make_folder("plot");
    // init lattice
    Lattice3D<Spin> lattice(L, L, L);
    lattice.randomize();
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
        metropolis(lattice,T,J,_inf_,Ns,{0,0,0},{0,0,0},
                MoveType::Random);
        // set 1 -> "001"
        std::ostringstream oss;
        oss << std::setw(3) << std::setfill('0') << it;
        std::string num = oss.str();
        // plot the lattice
        data::plot_lattice_small(lattice,"plot/Lattice_" + num
                + ".png");
        ++it;
    }
    // set the loading bar to 100%
    lbar.update(100);
    }
    cout << "finished ploting" << endl;
    // make a gif
    cout << "make gif ..."  << endl;
    data::convert_pngs_to_gif("plot/metropolis.gif", "plot/");
    cout << "finished" << endl;
    return 0;
}