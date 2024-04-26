#include <Wolf/wolf.h++>
#include <Ploting/PlotLattice.h++>
#include <Heisenberg.h++>
#include <Measure/Timekeeper.h++>

// temperature
F64 T = 0.1;
F64 J = 1.0;
F64 Time = 10.0;

int main()
{
    //              --- random seed
    const uint seed = 42;
    rng::set_seed(seed);

    //              --- Lattice
    Lattice lattice = Lattice::random_lattice(8, 8, 8, seed);

    //             --- adaptive-metropolis
    cout << "running wolf ..." << endl;
    wolf(lattice, T, Time);

    plot_lattice(lattice);

    return 0;
}