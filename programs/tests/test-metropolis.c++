#include <Metropolis/adaptive_metropolis.h++>
#include <Metropolis/metropolis.h++>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timekeeper.h++>
#include <Measure/Observables.h++>
#include <Eigen.h++>

// temperature
// F64 T = 0.001;
F64 J = 1.0;
F64 Time = 5;

float totalAbsolute(const Eigen::Matrix<float, 3, 1>& vec) {
    // Compute the sum of squares of each element
    float sum_of_squares = vec(0) * vec(0) + vec(1) * vec(1) + vec(2) * vec(2);

    // Return the square root of the sum of squares
    return std::sqrt(sum_of_squares);
}

int main()
{
    //              --- random seed
    const int seed = 42;
    rng::set_seed(seed);

    for (double T = 1; T <= 1.5; T += 0.1) {
        cout << "Temperature: " << T << endl;

        // Create a lattice
        Lattice lattice = Lattice::random_lattice(8, 8, 8);

        // Run adaptive metropolis
        cout << "Running adaptive metropolis..." << endl;
        adaptive_metropolis(lattice, T, J, T, maxUint);

        // Plot lattice
        //dat::plot_lattice(lattice);
        Vector3 mag = measure::get_magnetization(lattice);
        //std::cout << mag;
        std::cout << "\n";
        std::cout << totalAbsolute(mag)/512;

        // Create another lattice
        Lattice lattice1 = Lattice::random_lattice(8, 8, 8);

        // Run metropolis
        cout << "Running metropolis..." << endl;
        metropolis(lattice1, T, J, Time, maxUint);

        // Plot lattice
        //dat::plot_lattice(lattice1);
        Vector3 mag1 = measure::get_magnetization(lattice1);
        //std::cout <<  mag1;
        std::cout << "\n";
        std::cout << totalAbsolute(mag1)/512;
    }

    return 0;
}