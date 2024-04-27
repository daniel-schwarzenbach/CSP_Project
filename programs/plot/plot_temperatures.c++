#include <Metropolis/adaptive_metropolis.h++>
#include <Ploting/PlotLattice.h++>
#include <Heisenberg.h++>
#include <Measure/Timekeeper.h++>


// interaction
F64 J = 1.0;
// maximal runtime
F64 Time = 10.0;

int main()
{
    //              --- random seed
    const uint seed = 42;
    rng::set_seed(seed);
    make_folder("plots");
    //              --- Lattice
    Lattice lattice = Lattice::random_lattice(8, 8, 8, seed);

    // temperature
F64 T = 0.01;
    //             --- adaptive-metropolis
    cout << "running addaptive metropolis ..." << endl;
    for(uint i = 0; i < 8; i++){
        adaptive_metropolis(lattice, T, J, Time, maxUint);
        plot_lattice(lattice, get_filename("plots/", 8, J, 100, Time));
        T *= 10;
    }

    
    return 0;
}