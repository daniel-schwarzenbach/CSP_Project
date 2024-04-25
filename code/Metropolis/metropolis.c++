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
//      - trial move which we want to use to generate new spins: SpinFlip,
//        Random or SmallStep; default: small step move
//
// Output:
//      Returns true when the algorithm has finished running. The lattice
//      is modified throughout the runtime of the algorithm.
bool metropolis(Lattice &lattice, F64 T, F64 maxTimeSeconds, 
                uint maxSteps, MoveType moveType)
{
    // Start time and set step counter to 0
    uint step_count = 0;
    TimeKeeper watch;

    // Main Metropolis loop until number of steps or max time is reached
    // Check if max number of steps is reached
    while (step_count < maxSteps)
    {
        // Choose a random lattice site
        int x = rand() % lattice.Lx();
        int y = rand() % lattice.Ly();
        int z = rand() % lattice.Lz();

        // Get the spin at the chosen site (cartesian)
        Spin &spin = lattice(x, y, z);

        // Propose spin change based on given trial move
        Spin newSpin = spin;
        switch (moveType)
        {
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
        F64 deltaE = calculateEnergyDiff(lattice, x, y, z, spin, newSpin);

        // Acceptance condition
        if (deltaE <= 0 || rng::randf64() < exp(-deltaE / T))
        {                   // Boltzmann constant k is
                            // normalized with interaction strength J in this implementation
            spin = newSpin; // Accept the new configuration
        }

        // Check if maximum time has been reached
        if (watch.time() >= maxTimeSeconds)
        {
            break; // Stop simulation if maximum time reached
        }
        // Increase step counter
        ++step_count;
    }
    return true;
}