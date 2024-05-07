#ifndef __OBSERVABLES_H__
#define __OBSERVABLES_H__
/*
    physical observables
*/

#include <Heisenberg.h++>

namespace measure
{

    /*
    calculate the magnetization of all the spins inside the lattice

    / @brief
    / @param lattice our 3d lattice, where to perform the simulation
    / @return the non normalized magnetization of the lattice
    */
    Vector3 get_magnetization(const Lattice3D<Spin> &lattice);

    /*
    calculates the scalar average of the vector for every spin
    */
    flt get_scalar_average(Lattice3D<Spin> const &lattice,
                            Vector3 const &vec);


    /*
    calculates the correlation between two lattices
    */
    flt get_correlation(Lattice3D<Spin> const &prev,
                        Lattice3D<Spin> const &next);

    /*
    calculate the energy of all the spins inside the lattice

    / @brief
    / @param lattice our 3d lattice, where to perform the simulation
    on
    / @return the energy of the lattice
    */
    flt get_energy(const Lattice3D<Spin> &lattice, 
                   Vector3 const& h_vec = {0, 0, 0},
                   flt const& J = 1, Vector3 const& k_vec = {0,0,0});

    /*
    not tested!!!!!!!! danger
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