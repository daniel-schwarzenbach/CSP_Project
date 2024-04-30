#ifndef __OBSERVABLES_H__
#define __OBSERVABLES_H__

#include <Heisenberg.h++>

namespace measure
{

    /*
    calculate the magnetization of all the spins inside the lattice

    / @brief
    / @param lattice our 3d lattice, where to perform the simulation on
    / @return the non normalized magnetization of the lattice
    */
    Vector3 get_magnetization(const Lattice &lattice);

    flt get_scalar_average(Lattice const &lattice, Vector3 const &vec);

    flt get_auto_correlation(Lattice const &prev, Lattice const &next);

    /*
    calculate the energy of all the spins inside the lattice

    / @brief
    / @param lattice our 3d lattice, where to perform the simulation
    on
    / @return the energy of the lattice
    */
    flt get_energy(const Lattice &lattice, Vector3 h_vec = {0, 0, 0},
                   flt J = 1);

    /*
    calculate τ  s.d  min(|| yᵢ = exp(Tᵢ-T₀ / τ) ||²)

    / @brief
    / @param y Array<F64>
    / @param T Array<F64>
    on
    / @return the energy of the lattice
    */
    flt calculate_auto_correlation_time(Array<flt> const &y,
                                        Array<flt> const &T);

}

#endif // __OBSERVABLES_H__