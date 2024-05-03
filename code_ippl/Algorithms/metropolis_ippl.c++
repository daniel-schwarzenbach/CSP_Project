#include <Algorithms/Ippl_Algorithms.h++>
#include <LatticeIppl.h++>
#include <Measure/Timer.h++>

flt calculateEnergyDiff(LatticeIppl const& latticeIppl, int  const& x, 
                        int const& y,int const& z, 
                        Spin const& oldSpin, Spin const& newSpin, 
                        flt const& J, Spin const& h, Spin const& k){

    // Get dimensions of the latticeIppl
    Array<Index> neighbors = {
        {x + 1, y, z}, {x - 1, y, z}, {x, y + 1, z}, {x, y - 1, z}, 
        {x, y, z + 1}, {x, y, z - 1}};
    // Energies of old and new configuration
    flt energyOld = 0.0;
    flt energyNew = 0.0;
    for (Index neighbor : neighbors)
    {
        // Get indices of neighbors
        // Get neighboring spin
        Spin neighborSpin;
#pragma omp critical
        neighborSpin = latticeIppl(neighbor);

        // Calcualte and add energies
        energyOld += -J * (oldSpin | neighborSpin) 
                     - (oldSpin | h) 
                     - pow((oldSpin | k), 2);
        energyNew += -J * (newSpin | neighborSpin) 
                     - (newSpin | h) 
                     - pow((newSpin | k), 2);
    }   
    // Calculate energy difference (deltaE)
    flt deltaE = energyNew - energyOld;
    return deltaE;
}
// Metropolis algorithm //

// This function takes a reference to an initial latticeIppl and performs
// the Metropolis algorithm on this latticeIppl at the specified temperature.
// We can specify the trial move which we want to use. The algorithm
// stops when the max time OR the max number of steps, that are also
// specified by the input, are reached.

// Input:
//      - refrence to the latticeIppl
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
//      Returns true when the algorithm has finished running. The latticeIppl
//      is modified throughout the runtime of the algorithm.
bool metropolis_ippl(Lattice &latticeIppl,
                    flt const& T /*temperature*/,
                    flt const& J /*interaction Strength*/,
                    flt const& maxTimeSeconds,
                    uint const& maxSteps,
                    Spin const& h,
                    Spin const& k,
                    MoveType const& moveType)
{

#pragma omp parallel
    {
        uint numSteps = ceil(flt(maxSteps) / 
                        flt(omp_get_num_threads()));

        flt maxFactor = 60;
        measure::Timer watch;
        uint accepted_count = 0;
        flt sigma = maxFactor;
        uint Lx = latticeIppl.Lx();
        uint Ly = latticeIppl.Ly();
        uint Lz = latticeIppl.Lz();
        flt beta = Beta(T);
        watch.start();
        for (uint step = 0; step < numSteps; ++step)
        {
            // Choose a random latticeIppl site
            int x = rng::rand_int_range(0, Lx);
            int y = rng::rand_int_range(0, Ly);
            int z = rng::rand_int_range(0, Lz);

            // Get the spin at the chosen site (cartesian)
            Spin spin;
#pragma omp critical
            spin = latticeIppl(x, y, z);

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
            flt deltaE = calculateEnergyDiff(latticeIppl, x, y, z, spin,
                                             newSpin, J, h, k);
            
            // Acceptance condition
            if (deltaE <= 0 || rng::rand_uniform() < exp(-deltaE * beta))
            { // Boltzmann constant k is
              // normalized with interaction strength J in this implementation
              // Accept the new configuration
#pragma omp critical
                latticeIppl(x, y, z) = newSpin;
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

