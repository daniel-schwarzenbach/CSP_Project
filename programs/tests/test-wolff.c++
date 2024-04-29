#include <Wolff/wolff.h++>
#include <Data/Plot.h++>
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
    dat::plot_lattice(lattice, "bWolff.png");

    //             --- adaptive-metropolis
    cout << "running wolf ..." << endl;
    cout << wolff(lattice, T, J, Time, maxUint) << endl;

    dat::plot_lattice(lattice, "aWolff.png");

    return 0;
}