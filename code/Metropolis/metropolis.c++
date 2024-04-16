#include "metropolis.h++"
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

enum class MoveType { SpinFlip, Random, SmallStep };

using namespace std;
using namespace std::chrono;

// Forward declarations
template <class IpplT>
class LatticeIppl;

class SpinCartesian;
class SpinEigen;
class SpinPolar;

// Metropolis algorithm with time criterion
template <class IpplT>
bool metropolis(LatticeIppl<IpplT> &lattice, double maxTimeSeconds, MoveType moveType = MoveType::SmallStep) {
// Start time
    auto startTime = high_resolution_clock::now();
    
    // Main Metropolis loop
    while (true) {
        // Choose a random lattice site
        int x = rand() % lattice.Lx();
        int y = rand() % lattice.Ly();
        int z = rand() % lattice.Lz();

        // Get the spin at the chosen site
        IpplT& spin = lattice(x, y, z);
        
        // Propose spin change based on given trial move
        SpinCartesian newSpin;
        switch (moveType) {
            case MoveType::SpinFlip:
                newSpin = spin; // Spin flip move (reflect the spin)
                newSpin.spin_flip();
                break;
            case MoveType::Random:
                newSpin = SpinCartesian::random_move(); // Random move (generate a random spin)
                break;
            case MoveType::SmallStep:
                newSpin = spin.small_step_move(0.1); // Small step move (small change around the current spin)
                break;
        }
        // Calculate energy difference
        double deltaE = calculateEnergyDiff(lattice, x, y, z, oldSpin, newSpin);

        // Acceptance condition
        if (deltaE <= 0 || dis(gen) < exp(-deltaE)) { // TODO
            spin = newSpin; // Accept the new configuration
        }

        // Check elapsed time
        auto currentTime = high_resolution_clock::now();
        auto elapsedTime = duration_cast<duration<double>>(currentTime - startTime).count();
        
        // Check if maximum time has been reached
        if (elapsedTime >= maxTimeSeconds) {
            break; // Stop simulation if maximum time reached
        }
    }    
    return false;
}

template <class IpplT>

double calculateEnergyDiff(LatticeIppl<IpplT>& lattice, int x, int y, int z, IpplT& oldSpin, IpplT& newSpin) {
    constexpr double J = 1.0; // Interaction strength

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
        energyNew += -J * (oldSpin.x() * neighborSpin.x() + oldSpin.y() * neighborSpin.y() + oldSpin.z() * neighborSpin.z());
    }

    // Calculate energy difference (deltaE)
    double deltaE = energyNew - energyOld;
    return deltaE;
}

