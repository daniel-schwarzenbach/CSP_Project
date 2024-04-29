#include <Metropolis/adaptive_metropolis.h++>
#include <Metropolis/energy_diff.h++>

// Adaptive Metropolis algorithm //

// This function takes a reference to an initial lattice and performs
// the adaptive Metropolis algorithm on this lattice at the specified
// temperature. The algorithm stops when the max time OR the max 
// number of steps, that are also specified by the input, are reached.
// The trial move that is used in this version of the Metropolis 
// algorithm is similar to the small step move, which we already
// encountered in the standard Metropolis algorithm, combined with 
// a Gaussian move. To generate a trial spin we add a Gaussian distributed
// random vector multiplied by a factor sigma to the initial spin vector
// and then normalize the result to obtain the new spin. The adaptive
// element of the algorithm is the factor sigma that multiplies the 
// Gaussian distributed vector, which takes over the role of the opening
// angle in the small step move. This factor is changed after each step,
// depending on the acceptance rate in the previous step, to reach an 
// optimal acceptance rate of 50%. A more detailed description of
// the algorithm can be found in the report.


// Input: 
//      - reference to the lattice
//      - temperature T
//      - max running time of the algorithm
//      - max number of steps
//      - interaction strength of the Heisenberg model
//      - external magnetic field
//      - spatial anisotropy of the system
//      - initial and max factor of the Gaussian

// Output: 
//      Returns true when the algorithm has finished running. The lattice
//      is modified throughout the runtime of the algorithm.
bool adaptive_metropolis(Lattice &lattice, F64 T, F64 J, F64 maxTime,
                         uint maxSteps, Spin h, Spin k,
                         F64 maxFactor) {
    F64 sigma = maxFactor;
    TimeKeeper watch;
    int proposed_count = 0;
    F64 beta = Beta(T);

    // Main Metropolis loop until number of steps or max time is reached
    // Check if max number of steps is reached
    for(uint step = 0; step < maxSteps; ++step){
        // Choose a random lattice site
        int x = rand() % lattice.Lx();
        int y = rand() % lattice.Ly();
        int z = rand() % lattice.Lz();

        // Get the spin at the chosen site (cartesian)
        Spin& spin = lattice(x, y, z);
        //Initialize proposed spin
        Spin newSpin = spin;
        // Propose spin change based on the adaptive move
        newSpin.adaptive_step(sigma);
        // Calculate energy difference
        F64 deltaE = calculateEnergyDiff(lattice, x, y, z, spin, 
                                        newSpin, J, h, k);
        // increase count of proposed changes
        ++proposed_count;

        // Boltzmann constant k is normalized with interaction strength
        // J in this implementation
        // Acceptance condition
        if (deltaE <= 0 || rng::rand_f64() < exp(-deltaE * beta)) {
            // Accept the new configuration
            spin = newSpin;
            // Update the factor sigma based on the acceptance rate R:
            // The acceptance rate is given by: 1 / # proposed_count
            // The new sigma is then calculated by multiplying the
            // initial value with f = 0.5 / (1 - R).
            // To avoid division by zero we add a small number to the
            // denominator.
            sigma = std::min(maxFactor, sigma * float(0.5 / 
                            (1.0 - 1/proposed_count + 1e-8)));
            // Reset counter for proposed steps
            proposed_count = 0;
        }

        // Check if maximum time has been reached
        if (watch.time() >= maxTime) {
            break; // Stop simulation if maximum time reached
        }
    }    
    return true;
}