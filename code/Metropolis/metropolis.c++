#include "metropolis.h++"
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

enum class MoveType { SpinFlip, Random, SmallStep };

// Forward declarations
template <class IpplT>
class LatticeIppl;

class SpinCartesian;
class SpinEigen;
class SpinPolar;

// Metropolis algorithm
template <class IpplT>
bool metropolis(LatticeIppl<IpplT> &lattice, float T, float maxTimeSeconds, float maxSteps, MoveType moveType = MoveType::SmallStep) {
    // Initialize random number generator @ Daneil: wann machen wir das am besten??
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    // Start time and set step counter to 0
    int step_count = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

    // Main Metropolis loop until number of steps or max time is reached
    while (true) {
        // Choose a random lattice site
        int x = rand() % lattice.Lx();
        int y = rand() % lattice.Ly();
        int z = rand() % lattice.Lz();

        // Get the spin at the chosen site
        IpplT& spin = lattice(x, y, z);
        
        // Propose spin change based on given trial move
        SpinCartesian newSpin;
        newSpin = spin;
        switch (moveType) {
            case MoveType::SpinFlip:
                newSpin.spin_flip(); // Spin flip move (reflect the spin)
                break;
            case MoveType::Random:
                newSpin = newSpin.random_move(); // Random move (generate a random spin)
                break;
            case MoveType::SmallStep:
                newSpin = newSpin.small_step_move(0.1); // Small step move (small change around the current spin)
                break;
        }
        // Calculate energy difference
        double deltaE = calculateEnergyDiff(lattice, x, y, z, spin, newSpin);

        // Acceptance condition
        if (deltaE <= 0 || dis(gen) < exp(-deltaE / T)) { // Boltzmann constant k is 
        // normalized with interaction strength J in this implementation
            spin = newSpin; // Accept the new configuration
        }

        // Check elapsed time
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
        auto elapsed_time = elapsed_seconds.count(); // Get the count of elapsed seconds

        // Check if maximum time has been reached
        if (elapsed_time >= maxTimeSeconds) {
            break; // Stop simulation if maximum time reached
        }
        ++step_count;
    }    
    return false;
}

template <class IpplT>

double calculateEnergyDiff(LatticeIppl<IpplT>& lattice, int x, int y, int z, IpplT& oldSpin, IpplT& newSpin) {
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

    // Energy contribution from old spin configuration
    double energyOld = 0.0;
    for (int i = 0; i < 6; ++i) {
        int nx = neighbors[i][0];
        int ny = neighbors[i][1];
        int nz = neighbors[i][2];
        SpinCartesian& neighborSpin = lattice(nx, ny, nz);
        energyOld += -J * (oldSpin.x() * neighborSpin.x() + oldSpin.y() * neighborSpin.y() + oldSpin.z() * neighborSpin.z());
    }

    // Energy contribution from new spin configuration
    double energyNew = 0.0;
    for (int i = 0; i < 6; ++i) {
        int nx = neighbors[i][0];
        int ny = neighbors[i][1];
        int nz = neighbors[i][2];
        SpinCartesian& neighborSpin = lattice(nx, ny, nz);
        energyNew += -J * (newSpin.x() * neighborSpin.x() + newSpin.y() * neighborSpin.y() + newSpin.z() * neighborSpin.z());
    }

    // Calculate energy difference (deltaE)
    double deltaE = energyNew - energyOld;
    return deltaE;
}
