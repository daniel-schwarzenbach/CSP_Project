#include <Metropolis/metropolis.h++>
#include <Metropolis/energy_diff.h++>
#include <Measure/Timer.h++>

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
// depending on the acceptance rate, to reach an optimal acceptance rate 
// of 50%. A more detailed description of the algorithm can be found 
// in the report.

flt sigma = 60;
u64 totalSteps = 0;
u64 acceptedCount = 0;

void restet_adaptive(){
    flt sigma = 60;
    u64 totalSteps = 0;
    u64 acceptedCount = 0;
}

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
bool adaptive_metropolis(   Lattice3D<Spin> &lattice,
                            flt const& T /*temperature*/,
                            flt const& J /*interaction Strength*/,
                            flt const& maxTimeSeconds,
                            u64 const& maxSteps,
                            Spin const& h,
                            Spin const& k,
                            flt const& maxFactor){
    // Initialize sigma with the max value, start timer, calculate 
    // Boltzmann factor and initialize counter of accepted steps;
    measure::Timer watch;
    flt beta = Beta(T);
    // Main Metropolis loop until number of steps or max time is reached
    // Check if max number of steps is reached
    for(u64 step = 0; step < maxSteps; ++step){
        ++totalSteps;
        // Choose a random lattice site
        int x = rng::rand_int_range(0, lattice.Lx());
        int y = rng::rand_int_range(0, lattice.Ly());
        int z = rng::rand_int_range(0, lattice.Lz());

        // Get the spin at the chosen site (cartesian)
        Spin &spin = lattice(x, y, z);
        // Initialize proposed spin
        Spin newSpin = spin;
        // Propose spin change based on the adaptive move
        newSpin.adaptive_step(sigma);
        // Calculate energy difference
        flt deltaE = calculateEnergyDiff(lattice, x, y, z, spin, 
                                        newSpin, J, h, k);
        // Acceptance condition
        if (deltaE <= 0 || rng::rand_uniform() < exp(-deltaE * beta)) {
            // Accept the new configuration
            spin = newSpin.normalized();
            // Update the factor sigma based on the acceptance rate R:
            // The acceptance rate is given by the ratio of the accepted
            // steps and the proposed steps.
            // The new sigma is then calculated by multiplying the
            // initial value with f = 0.5 / (1 - R).
            // To avoid division by zero in the first steps, we add a
            //small number to the denominator.

            // Increase counter of accepted steps
            ++acceptedCount;
            // Update acceptance rate
            flt R = flt(acceptedCount)/flt(totalSteps+1.0);
            // Calculate update factor
            flt f = 0.5 / (1.0 - R + 1e-18);
            // Update sigma
            sigma = std::min(maxFactor, sigma * f );
        }
        // Check if maximum time has been reached
        if (watch.time() >= maxTimeSeconds)
        {
            break; // Stop simulation if maximum time reached
        }
    }
    return true;
}
