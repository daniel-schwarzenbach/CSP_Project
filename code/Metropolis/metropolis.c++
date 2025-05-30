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
//      - trial move which we want to use to generate new spins: SpinFlip,
//        Random or SmallStep; default: small step move
//
// Output:
//      Returns true when the algorithm has finished running. The lattice
//      is modified throughout the runtime of the algorithm.
bool metropolis(Lattice3D<Spin> &lattice,
                flt const& T /*temperature*/,
                flt const& J /*interaction Strength*/,
                flt const& maxTimeSeconds,
                u64 const& maxSteps,
                Spin const& h,
                Spin const& k,
                MoveType const& moveType) {
    // Start time and set step counter to 0
    u64 step_count = 0;
    measure::Timer watch;
    flt beta = Beta(T); 

    // Main Metropolis loop until number of steps or max time is reached
    // Check if max number of steps is reached
    for(int64_t step = 0; step < maxSteps; ++step){
        // Choose a random lattice site
        int x = rng::rand_int_range(0, lattice.Lx());
        int y = rng::rand_int_range(0, lattice.Ly());
        int z = rng::rand_int_range(0, lattice.Lz());

        // Get the spin at the chosen site (cartesian)
        Spin &spin = lattice(x, y, z);

        // Propose spin change based on given trial move
        Spin newSpin = spin;
        switch (moveType)
        {
        case MoveType::SpinFlip:
            newSpin.spin_flip_step(); // Spin flip move (reflect the spin)
            break;
        case MoveType::Random:
            newSpin.random_step(); // Random move (generate a random spin)
            break;
        case MoveType::SmallStep:
            newSpin.small_step(0.2); // Small step move (small change around the current spin)
            break;
        case MoveType::Addaptive:
            cerr << ERROR 
<< "not implemented! use adaptive metropolis instead!" << endl;
            return false;
            break;
        }
        // Calculate energy difference
        flt deltaE = calculateEnergyDiff(lattice, x, y, z, spin, 
                                        newSpin, J, h, k);
 
        if (deltaE <= 0 || rng::rand_uniform() < exp(-deltaE * beta)){
            spin = newSpin;
        }
        // Check if maximum time has been reached
        if (watch.time() >= maxTimeSeconds)
        {
            break; // Stop simulation if maximum time reached
        }
    }
    return true;
}