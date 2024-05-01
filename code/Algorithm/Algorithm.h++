#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include <Metropolis/metropolis.h++>
#include <Wolff/wolff.h++>
#include <Heisenberg.h++>
#include <Measure/LoadingBar.h++>
#include <Measure/Observables.h++>

namespace algo{


static constexpr uint J = 0;

using AlgoData = StaticArray<Array<flt>, 4>;

/*
Function to measure after a certain time step

/ @param lattice: lattice to work with
/ @param dt: timestep
/ @param T: temperature
*/
using FunctionDeltaT=function<void(Lattice&, flt const&, flt const&)>;

static FunctionDeltaT wolff_singleCore
        = [](Lattice& lattice, flt const& dt, flt const& T){
    wolff(lattice, T, J, dt, maxUint);
};

static FunctionDeltaT metropolis_smallStep
        = [](Lattice& lattice, flt const& dt, flt const& T){
    metropolis(lattice, T, J, dt, maxUint, MoveType::SmallStep);
};

static FunctionDeltaT metropolis_adaptive
        = [](Lattice& lattice, flt const& dt, flt const& T){
    metropolis(lattice, T, J, dt, maxUint, MoveType::SmallStep);
};

static FunctionDeltaT metropolis_random
        = [](Lattice& lattice, flt const& dt, flt const& T){
    metropolis(lattice, T, J, dt, maxUint, MoveType::Random);
};

/*
function that runs the algorithm and collects the datat after every
time step dt

@param dt: time step size
@param t_end: simulation time
@param T: temperature
@return AlgoData: {time, magnetization, energy, autocorolation}

*/
AlgoData test_function_delta_t(
        Lattice& lattice,
        flt const &dt, flt const &t_end, flt const& T,
        function<void(Lattice&, flt const&, flt const&)> const &algo);

}
#endif // __ALGORITHM_H__