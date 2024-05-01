#include <Wolff/wolff_ghost.h++>
#include <Wolff/wolff.h++>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timekeeper.h++>
#include <Measure/Observables.h++>
#include <fstream>

// temperature
F64 T = 2.0;
F64 J = 1.0;
F64 Time = 3.0;
Spin h = Spin(0.0,0.0,0.0);

int main()
{
    //              --- random seed
    const uint seed = 80;
    rng::set_seed(seed);

    //              --- Lattice
    Lattice lattice = Lattice::random_lattice(8, 8, 8);
    //dat::plot_lattice(lattice, "Wolff_start.png");

    int Nmax = 1000;

    //Plot magnetisation and energy as a function of steps to determine N_eq (until equilibrium)
    Array<Vector3> magnetisations(Nmax); 
    Array<F64> energies(Nmax); 

    std::ofstream outFile("magnetizations.txt");

    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }

    for (int i = 0; i < Nmax; ++i){
        wolff(lattice, T, J, Time, 100); //check reference
        Vector3 magnetisation = measure::get_magnetization(lattice);
        flt energy = measure::get_energy(lattice, h, J);
        magnetisations[i] = magnetisation;
        flt mag = magnetisations[i].norm();
        energies[i] = energy;

        outFile << mag << std::endl;
    }

    outFile.close();

    //Compute means and std to determine the moments of magnetisation and Energy (e.g. specific heat, binder cumulant)

    //Determine N_eq for different temperatures and take linear ramp Eq. 42, 43
    // for i in Temperature...


    //ASSUMING 

    //             --- wolff-ghost
    // cout << "running wolff ghost ..." << endl;
    // wolff_ghost(lattice, T, J, Time, maxUint);

    // dat::plot_lattice(lattice, "Wolff_ghost_end.png");

    // rng::set_seed(seed);
    // lattice.randomize();

    // dat::plot_lattice(lattice, "Wolff_start_compare.png");

    // wolff(lattice, T, J, Time, maxUint);

    // dat::plot_lattice(lattice, "Wolff_end.png")
    return 0;
}

