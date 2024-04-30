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
const flt J = 1.0;
// maximal runtime
const flt dt = 10.0;
const flt at = 60.0;
// end Time
const flt time = 120.0;

const flt dT = 0.1;
// int Random Lattice Seed
const int seed = 69; // hi hi

// side Lenth
const uint L = 16;

int main(int mainArgCount, char **mainArgs)
{
    rng::set_seed(seed);
    Lattice lattice = Lattice::random_lattice(L, L, L);

    uint tmpSize = 100;
    Array<flt> tmp(tmpSize);

    Array<flt> nm_mag(tmpSize);
    Array<flt> nm_nrg(tmpSize);
    Array<flt> nm_mag_err(tmpSize);
    Array<flt> nm_nrg_err(tmpSize);

    Array<flt> am_mag(tmpSize);
    Array<flt> am_nrg(tmpSize);
    Array<flt> am_mag_err(tmpSize);
    Array<flt> am_nrg_err(tmpSize);

    Array<flt> wo_mag(tmpSize);
    Array<flt> wo_nrg(tmpSize);
    Array<flt> wo_mag_err(tmpSize);
    Array<flt> wo_nrg_err(tmpSize);

    for (uint i = 0; i < tmpSize; ++i)
    {
        uint timeSize = 10;

        Array<flt> nm_mag(timeSize);
        Array<flt> nm_nrg(timeSize);
        Array<flt> nm_aut(timeSize);

        Array<flt> am_mag(timeSize);
        Array<flt> am_nrg(timeSize);
        Array<flt> am_aut(timeSize);

        Array<flt> wo_mag(timeSize);
        Array<flt> wo_nrg(timeSize);
        Array<flt> wo_aut(timeSize);

        rng::set_seed(seed);
        lattice.randomize();
        for (uint j = 0; j < timeSize; ++j)
        {
            nm_mag
        }
    }
}