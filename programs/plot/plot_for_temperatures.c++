#include <Metropolis/adaptive_metropolis.h++>
#include <Metropolis/metropolis.h++>
#include <Wolff/wolff.h++>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timekeeper.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <matplot/matplot.h>
#include <LoadingBar.h++>

namespace plt = matplot;

// interaction
const F64 J = 1.0;
// maximal runtime
const F64 dt = 10.0;
const F64 at = 60.0;
// end Time
const F64 time = 120.0;

const F64 dT = 0.1;
// int Random Lattice Seed
const int seed = 69; // hi hi

// side Lenth
const uint L = 16;

int main(int mainArgCount, char **mainArgs){
    rng::set_seed(seed);
    Lattice lattice = Lattice::random_lattice(L,L,L);

    uint tmpSize = 100;
    Array<F64> tmp(tmpSize);

    Array<F64> nm_mag(tmpSize);
    Array<F64> nm_nrg(tmpSize);
    Array<F64> nm_mag_err(tmpSize);
    Array<F64> nm_nrg_err(tmpSize);

    Array<F64> am_mag(tmpSize);
    Array<F64> am_nrg(tmpSize);
    Array<F64> am_mag_err(tmpSize);
    Array<F64> am_nrg_err(tmpSize);

    Array<F64> wo_mag(tmpSize);
    Array<F64> wo_nrg(tmpSize);
    Array<F64> wo_mag_err(tmpSize);
    Array<F64> wo_nrg_err(tmpSize);
    
    for (uint i = 0; i < tmpSize; ++i){
        uint timeSize = 10;
        
        Array<F64> nm_mag(timeSize);
        Array<F64> nm_nrg(timeSize);
        Array<F64> nm_aut(timeSize);

        
        Array<F64> am_mag(timeSize);
        Array<F64> am_nrg(timeSize);
        Array<F64> am_aut(timeSize);

        
        Array<F64> wo_mag(timeSize);
        Array<F64> wo_nrg(timeSize);
        Array<F64> wo_aut(timeSize);

        rng::set_seed(seed);
        lattice.randomize();
        for(uint j = 0; j < timeSize; ++j){
            nm_mag
        }
    }
}