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
//      - trial move which we want to use to generate new spins: SpinFlip,
//        Random or SmallStep; default: small step move
//
// Output:
//      Returns true when the algorithm has finished running. The lattice
//      is modified throughout the runtime of the algorithm.
bool metropolis(Lattice &lattice,
                flt const& T /*temperature*/,
                flt const& J /*interaction Strength*/,
                flt const& maxTimeSeconds,
                uint const& maxSteps,
                MoveType const& moveType)
{
    // max factor

    // Main Metropolis loop until number of steps or max time is reached
    // Check if max number of steps is reached

#pragma omp parallel
    {

        uint numSteps = ceil(flt(maxSteps) / flt(omp_get_num_threads()));

        flt maxFactor = 20;
        measure::Timer watch;
        uint proposed_count = 0;
        flt sigma = maxFactor;
        uint Lx = lattice.Lx();
        uint Ly = lattice.Ly();
        uint Lz = lattice.Lz();
        watch.start();
        for (uint step = 0; step < numSteps; ++step)
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
                newSpin = Spin::get_random(); // Random move (generate a random spin)
                break;
            case MoveType::SmallStep:
                newSpin.small_step(0.2); // Small step move (small change around the current spin)
                break;
            case MoveType::Addaptive:
                newSpin.adaptive_step(sigma);
            }
            // Calculate energy difference
            flt deltaE = calculateEnergyDiff(lattice, x, y, z, spin,
                                             newSpin, J);
            // increase count of proposed changes
            ++proposed_count;
            // Acceptance condition
            flt beta = Beta(T);

            if (deltaE <= 0 || rng::rand_uniform() < exp(-deltaE * beta))
            { // Boltzmann constant k is
              // normalized with interaction strength J in this implementation
              // Accept the new configuration
#pragma omp critical
                {
                    lattice(x, y, z) = newSpin;
                }
                sigma = std::min(maxFactor, sigma * 0.5 /
                        (1.0 - (1.0 / proposed_count) + 1e-2));

                proposed_count = 0;
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