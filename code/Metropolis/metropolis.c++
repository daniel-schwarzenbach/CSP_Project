#include "metropolis.h++"
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Forward declarations
template <class IpplT>
class LatticeIppl;

class SpinCartesian;
class SpinEigen;
class SpinPolar;

// Metropolis function with time criterion
template <class IpplT>

bool metropolis(Lattice & lattice){
    // Generate random initial configuration
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 1.0);

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

        // Make a trial move (flip spin)
        IpplT oldSpin = spin;
        IpplT newSpin = trialMove(spin);

        // Calculate energy difference
        double deltaE = calculateEnergy(lattice, x, y, z, oldSpin, newSpin);

        // Acceptance condition
        if (deltaE <= 0 || dis(gen) < exp(-deltaE)) {
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


    // Calculate energy of configuration (not necessary)
    // Make the energy of the lattice an instance of the class??
    // Other observables maybe as well

    // While algo is not converged, i.e. thermal equilibrium is not
    // reached
        // Choose random spin
     
        //  Make trial move: call trial move function

        // Calculate new energy
        // For efficiency: maybe not calculate energy of the new
        // configuration but just energy difference, since most
        // of the cluster actually remains unchanged

        // Accept with probability depending on energy difference

    // Check for convergence: criterion??? ideas
    // - mean square deviation of the energy over the past steps
    // - same with magnetization
    // - same with specific heat
    // - Binder cumulant
    // - autocorrelation time

    // Maybe not check for convergence after every step, but every 100
    // or so, or implement an adaptive check:
    // - Exponential/linear decrease of steps between checks
    // - Adaptive number of steps depending on how far away the system
    // is from the convergence theshold


    // Other questions: 
    // - implement class instances for observables??
    // - how to plot energy, magnetization and other observables??
    
    return false;
}

template <class IpplT>

double calculateEnergy(LatticeIppl<IpplT>& lattice, int x, int y, int z, IpplT& oldSpin, IpplT& newSpin) {
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

