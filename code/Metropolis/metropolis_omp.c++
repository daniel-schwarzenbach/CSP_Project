#include <Metropolis/metropolis.h++>
#include <Metropolis/energy_diff.h++>

// Metropolis algorithm //

// This function takes a reference to an initial lattice and performs
// the Metropolis algorithm on this lattice at the specified temperature.
// We can specify the trial move which we want to use. The algorithm
// stops when the max time OR the max number of steps, that are also
// specified by the input, are reached.

flt sigma_omp = 100;
u64 totalSteps_omp = 0;
u64 acceptedCount_omp = 0;
constexpr flt maxFactor_omp = 60;

void restet_adaptive_omp(){
    flt sigma_omp = 100;
    u64 totalSteps_omp = 0;
    u64 acceptedCount_omp = 0;
}

bool metropolis_omp(Lattice &lattice,
                    flt const& T /*temperature*/,
                    flt const& J /*interaction Strength*/,
                    flt const& maxTimeSeconds,
                    u64 const& maxSteps,
                    Spin const& h,
                    Spin const& k,
                    MoveType const& moveType)
{
    measure::Timer watch;
    const uint Lx = lattice.Lx();
    const uint Ly = lattice.Ly();
    const uint Lz = lattice.Lz();
    const flt beta = Beta(T);
    flt simga_total = 0.0;
    #pragma omp parallel
    {
    u64 thread_totalSteps = totalSteps_omp;
    u64 thread_acceptedCount = acceptedCount_omp;
    flt thread_sigma = sigma_omp;
    #pragma omp for schedule (dynamic, 50)
    for (u64 step = 0; step < maxSteps; ++step){
        ++thread_totalSteps;
        // Choose a random lattice site
        int x = rng::rand_int_range(0, Lx);
        int y = rng::rand_int_range(0, Ly);
        int z = rng::rand_int_range(0, Lz);
        // Get the spin at the chosen site (cartesian)
        Spin spin;
        //#pragma omp atomic update
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
                newSpin.adaptive_step(sigma_omp);
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
                ++thread_acceptedCount;
                // Update acceptance rate
                const flt R = acceptedCount_omp/(totalSteps_omp+1.0);
                // Calculate update factor
                const flt f = 0.5 / (1.0 - R + 1e-18);
                // Update sigma   
                thread_sigma = std::min(maxFactor_omp, thread_sigma*f);
            }
        }

        // Check if maximum time has been reached
        if (watch.time() >= maxTimeSeconds)
        {
            #ifndef WITH_OPENMP
            break;
            #endif
            #pragma omp cancel for // Stop simulation if maximum time reached
        }
    } // end for
    if(moveType == MoveType::Addaptive){
        
        #pragma omp critical  
        {
        totalSteps_omp += thread_totalSteps;
        acceptedCount_omp += thread_acceptedCount;
        simga_total += thread_sigma;
        } // end critical
    }
    } // end parrallel
    sigma_omp = simga_total / omp_get_num_threads();
    return true;
}