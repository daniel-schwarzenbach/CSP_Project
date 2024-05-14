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
// plot after 50 steps
const u64 Ns = 100;
// final number of steps
const u64 Nmax = 10'000;
// external Magnetic field
const Spin h = {0,0,0};
// anisotropy Vector
const Spin k = {0,0,0};

int main(int mainArgCount, char **mainArgs)
{
    
    
    // init lattice
    Lattice3D<Spin> lattice(L, L, L);
    lattice.set_constant({0,0,-1});
    
    // start 
    Array<flt> Ts = {0.1, 1.5, 5.0};
    for(flt T : Ts){
        uint it = 0; // to name files
        // start with a constant lattice
        lattice.set_constant({0,0,1});
        // make folder to store the plots
        string folder = "plot" + to_string(uint(T*10));
        data::make_folder(folder);
        
        cout << "start Ploting ..."  << endl;
        // init loading bar
        {
        // plot starting lattice
        std::ostringstream oss;
            oss << std::setw(3) << std::setfill('0') << it;
            std::string num = oss.str();
        data::plot_lattice(lattice,folder + "/Lattice_" + num
                    + ".png");
        }
        {
        measure::LoadingBar lbar(60);
        for (u64 i = 0; i < Nmax; i+=Ns)
        {
            ++it;
            // updade the loading bar
            lbar.update((i * 100.0) / Nmax);
            // do Ns steps
            metropolis(lattice,T,J,_inf_,Ns,h,k,
                    MoveType::Random);
            // set 1 -> "001"
            std::ostringstream oss;
            oss << std::setw(3) << std::setfill('0') << it;
            std::string num = oss.str();
            // plot the lattice
            data::plot_lattice(lattice,folder+"/Lattice_" + num
                    + ".png");
            
        }
        // set the loading bar to 100%
        lbar.update(100);
        }
        cout << "finished ploting" << endl;
        // make a gif
        cout << "make gif ..."  << endl;
        data::convert_pngs_to_gif(folder+"/metropolis.gif", folder+"/");
        cout << "finished" << endl << endl;

    }
    return 0;
}