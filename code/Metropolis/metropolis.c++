#include <Timekeeper.h++>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <Heisenberg.h++>
#include <Metropolis/metropolis.h++>

double calculateEnergyDiff(Lattice& lattice, int x, int y, int z, Spin& oldSpin, Spin& newSpin);

// Metropolis algorithm
bool metropolis(Lattice& lattice, float T, float maxTimeSeconds, float maxSteps, MoveType moveType) {
    // Initialize random number generator @ Daniel: wann machen wir das am besten??
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    // Start time and set step counter to 0
    int step_count = 0;
    TimeKeeper watch;

    // Main Metropolis loop until number of steps or max time is reached
    while (true) {
        // Choose a random lattice site
        int x = rand() % lattice.Lx();
        int y = rand() % lattice.Ly();
        int z = rand() % lattice.Lz();

        // Get the spin at the chosen site (cartesian)
        Spin& spin = lattice(x, y, z);
        
        // Propose spin change based on given trial move
        SpinCartesian newSpin = spin;
        switch (moveType) {
            case MoveType::SpinFlip:
                newSpin.spin_flip(); // Spin flip move (reflect the spin)
                break;
            case MoveType::Random:
                newSpin.random_move(); // Random move (generate a random spin)
                break;
            case MoveType::SmallStep:
                newSpin.small_step_move(0.1); // Small step move (small change around the current spin)
                break;
        }
        // Calculate energy difference
        double deltaE = calculateEnergyDiff(lattice, x, y, z, spin, newSpin);

        // Acceptance condition
        if (deltaE <= 0 || dis(gen) < exp(-deltaE / T)) { // Boltzmann constant k is 
        // normalized with interaction strength J in this implementation
            spin = newSpin; // Accept the new configuration
        }

        // Check if maximum time has been reached
        if (watch.time() >= maxTimeSeconds) {
            break; // Stop simulation if maximum time reached
        }
        ++step_count;
    }    
    return true;
}



double calculateEnergyDiff(Lattice& lattice, int x, int y, int z, Spin& oldSpin, Spin& newSpin) {
    constexpr double J = 1.0; // Interaction strength, normalized with k_b

    // Get dimensions of the lattice
    int Lx = lattice.Lx();
    int Ly = lattice.Ly();
    int Lz = lattice.Lz();

    // Indices of nearest neighbors (periodic boundary conditions assumed)
    int neighbors[6][3] = {
        {(x + 1) % Lx, y, z}, {(x - 1 + Lx) % Lx, y, z}, // +x, -x neighbors
        {x, (y + 1) % Ly, z}, {x, (y - 1 + Ly) % Ly},    // +y, -y neighbors
        {x, y, (z + 1) % Lz}, {x, y, (z - 1 + Lz) % Lz}  // +z, -z neighbors
    };

    // Energies of old and new configuration
    double energyOld = 0.0;
    double energyNew = 0.0;
    for (int i = 0; i < 6; ++i) {
        // Get indices of neighbors
        int nx = neighbors[i][0];
        int ny = neighbors[i][1];
        int nz = neighbors[i][2];
        // Get neighboring spin
        SpinCartesian& neighborSpin = lattice(nx, ny, nz);
        // Calcualte and add energies
        energyOld += -J * (oldSpin.x() * neighborSpin.x() + oldSpin.y() * neighborSpin.y() + oldSpin.z() * neighborSpin.z());
        energyNew += -J * (newSpin.x() * neighborSpin.x() + newSpin.y() * neighborSpin.y() + newSpin.z() * neighborSpin.z());
    }   

    // Calculate energy difference (deltaE)
    double deltaE = energyNew - energyOld;
    return deltaE;
}
