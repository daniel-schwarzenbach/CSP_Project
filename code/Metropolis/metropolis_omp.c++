#include <Metropolis/metropolis.h++>
#include <Metropolis/energy_diff.h++>

// Metropolis algorithm //

// This function takes a reference to an initial lattice and performs
// the Metropolis algorithm on this lattice at the specified temperature.
// We can specify the trial move which we want to use. The algorithm
// stops when the max time OR the max number of steps, that are also
// specified by the input, are reached.

// Input:
//      - refrence to the lattice
//      - temperature T
//      - maximal running time of the algorithm
//      - maximal number of steps
//      - interaction strength of the Heisenberg model
//      - external magnetic field
//      - spatial anisotropy of the system
//      - trial move which we want to use to generate new spins:SpinFlip,
//        Random or SmallStep; default: small step move
//
// Output:
//      Returns true when the algorithm has finished running. The lattice
//      is modified throughout the runtime of the algorithm.
bool metropolis_omp(Lattice &lattice,
                    flt const& T /*temperature*/,
                    flt const& J /*interaction Strength*/,
                    flt const& maxTimeSeconds,
                    u64 const& maxSteps,
                    Spin const& h,
                    Spin const& k,
                    MoveType const& moveType)
{

#pragma omp parallel
    {

        u64 numSteps = ceil(flt(maxSteps) / 
                        flt(omp_get_num_threads()));

        flt maxFactor = 100;
        measure::Timer watch;
        uint accepted_count = 0;
        flt sigma = maxFactor;
        uint Lx = lattice.Lx();
        uint Ly = lattice.Ly();
        uint Lz = lattice.Lz();
        flt beta = Beta(T);
        watch.start();
        for (u64 step = 0; step < numSteps; ++step)
        {
            // Choose a random lattice site
            int x = rng::rand_int_range(0, Lx);
            int y = rng::rand_int_range(0, Ly);
            int z = rng::rand_int_range(0, Lz);

            // Get the spin at the chosen site (cartesian)
            Spin spin;
#pragma omp critical
            spin = lattice(x, y, z);

            // Propose spin change based on given trial move
            Spin newSpin = spin;
            // do the requested move
            switch (moveType)
            {
                case MoveType::SpinFlip:
                    newSpin.spin_flip_step(); // Spin flip move (reflect the spin)
                    break;
                case MoveType::Random:
                    newSpin.random_step(); // Random move (generate a random spin)
                    break;
                case MoveType::SmallStep:
                    newSpin.small_step(0.1); // Small step move (small change around the current spin)
                    break;
                case MoveType::Addaptive:
                    newSpin.adaptive_step(sigma);
                    break;
                }
            // Calculate energy difference
            flt deltaE = calculateEnergyDiff(lattice, x, y, z, spin,
                                             newSpin, J, h, k);
            
            // Acceptance condition
            if (deltaE <= 0 || rng::rand_uniform() < exp(-deltaE * beta))
            { // Boltzmann constant k is
              // normalized with interaction strength J in this implementation
              // Accept the new configuration
#pragma omp critical
                lattice(x, y, z) = newSpin;
                if(moveType == MoveType::Addaptive){
                    ++accepted_count;
                    // Update acceptance rate
                    flt R = accepted_count/(step+1.0);
                    // Calculate update factor
                    flt f = 0.5 / (1.0 - R + 1e-18);
                    // Update sigma
                    sigma = std::min(maxFactor, sigma * f );
                }
            }

            // Check if maximum time has been reached
            if (watch.time() >= maxTimeSeconds)
            {
                break; // Stop simulation if maximum time reached
            }
        }
    } // end parrallel

    return true;
}