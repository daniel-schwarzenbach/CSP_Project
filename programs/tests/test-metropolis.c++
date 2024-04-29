#include <Metropolis/adaptive_metropolis.h++>
#include <Metropolis/metropolis.h++>
#include <Ploting/PlotLattice.h++>
#include <Heisenberg.h++>
#include <Measure/Timekeeper.h++>

// temperature
F64 T = 10001.0;
F64 J = 1.0;
F64 Time = 10.0;

int main()
{
    //              --- random seed
    const uint seed = 42;
    rng::set_seed(seed);

    //              --- Lattice
    Lattice lattice = Lattice::random_lattice(8, 8, 8, seed);
    plot_lattice(lattice);

    //             --- metropolis
    cout << "running adaptive metropolis ..." << endl;
    adaptive_metropolis(lattice, T, J, Time, maxUint);

    plot_lattice(lattice);

    Lattice lattice1 = Lattice::random_lattice(8, 8, 8, seed);
    plot_lattice(lattice1);
    cout << "running metropolis ..." << endl;
    metropolis(lattice1, T, J, Time, maxUint);

    plot_lattice(lattice1);

    return 0;
}