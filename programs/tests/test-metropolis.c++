#include <Metropolis/metropolis.h++>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timekeeper.h++>

// temperature
F64 T = 10000.1;
F64 J = 1.0;
F64 Time = 10.0;

int main()
{
    //              --- random seed
    const uint seed = 42;
    rng::set_seed(seed);

    //              --- Lattice
    Lattice lattice = Lattice::random_lattice(8, 8, 8);
    dat::plot_lattice(lattice, "Metropolis_start.png");

    //             --- metropolis
    cout << "running metropolis ..." << endl;
    metropolis(lattice, T, J, Time, maxUint);

    dat::plot_lattice(lattice, "Metropolis_end.png");

    return 0;
}