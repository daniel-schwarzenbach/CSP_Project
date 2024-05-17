#include <Measure/Observables.h++>

#include <numeric>
#include <algorithm>

Vector3 measure::get_magnetization(const Lattice3D<Spin> &lattice)
{

    // Define the magnetization as a vector
    // e. g. magnetization[0] = x-component of the vector M
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
    f32 N = Lx * Ly * Lz;
    // sum up all values
    f32 sx = 0; f32 sy = 0; f32 sz = 0;
    #pragma omp parallel for reduction(+:sx, sy, sz)
    for (int x = 0; x < Lx; x++)
    {
        for (int y = 0; y < Ly; y++)
        {
            for (int z = 0; z < Lz; z++)
            {
                sx += lattice(x, y, z)(0);
                sy += lattice(x, y, z)(1);
                sz += lattice(x, y, z)(2);
            }
        }
    }
    //  normalize the values and return them
    return {sx/N, sy/N, sz/N};
}

flt measure::get_energy(const Lattice3D<Spin> &lattice, 
                        Vector3 const& h_vec, 
                        flt const& J, Vector3 const& k_vec)
{

    /*
    Define the energy as a scalar (f32)
    given by the Hamiltonian:

    H = -J 𝚺_{<i,j>}{ 𝚺_{α ∈ (x,y,z)} (S_i)_α * (S_j)_α
                    - 𝚺_i 𝚺_{α ∈ (x,y,z)} (h)_α * (S)_α }

    The first term is a summation over all possible bonds (depending
    on the boundary conditions in the lattice, while the second term
    is similar to the magnetization.
    */

    flt spin_interaction_energy = 0.0;
    flt mag_interaction_energy = 0.0;
    flt anisotropy_energy = 0.0;

    /*
    If we have periodic (p) bondary conditions, then bond_factor = 0 
    and we sum over all possible bonds, including the interaction of 
    the spin at (Lx, 1, 1) along the bond 
    parallel to the x-direction
    with the spin at (Lx+1 = 1, 1, 1).
    If we have open (o) boundary conditions, then bond_factor = 1 and 
    we only sum over all internal bonds.

    The magnetic interaction energy is not affected by the boundary 
    conditions.
    */
    // get dims
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
// sum over all bonds parallel to x-direction
#pragma omp parallel for reduction(+ : spin_interaction_energy)
    for (int x = 0; x < Lx; x++)
    {
        for (int y = 0; y < Ly; y++)
        {
            for (int z = 0; z < Lz; z++)
            {
                spin_interaction_energy +=
                        lattice(x, y, z) | lattice(x + 1, y, z);
            }
        }
    }

// sum over all bonds parallel to y-direction
#pragma omp parallel for reduction(+ : spin_interaction_energy)
    for (int x = 0; x < Lx; x++)
    {
        for (int y = 0; y < Ly; y++)
        {
            for (int z = 0; z < Lz; z++)
            {
                spin_interaction_energy +=
                        lattice(x, y, z) | lattice(x, y + 1, z);
            }
        }
    }

// sum over all bonds parallel to z-direction
#pragma omp parallel for reduction(+ : spin_interaction_energy)
    for (int x = 0; x < Lx; x++)
    {
        for (int y = 0; y < Ly; y++)
        {
            for (int z = 0; z < Lz; z++)
            {
                spin_interaction_energy +=
                    lattice(x, y, z) | lattice(x, y, z + 1);
            }
        }
    }

// sum over spins to calculate the interaction with the magnetic field
#pragma omp parallel for reduction(+ : mag_interaction_energy)
    for (int x = 0; x < Lx; x++)
    {
        for (int y = 0; y < Ly; y++)
        {
            for (int z = 0; z < Lz; z++)
            {
                mag_interaction_energy += h_vec | lattice(x, y, z);
            }
        }
    }

#pragma omp parallel for reduction(+ : mag_interaction_energy)
    for (int x = 0; x < Lx; x++)
    {
        for (int y = 0; y < Ly; y++)
        {
            for (int z = 0; z < Lz; z++)
            {
                anisotropy_energy +=pow (k_vec | lattice(x, y, z),2);
            }
        }
    }
    // return all summed up enegries
    return (-J * spin_interaction_energy - mag_interaction_energy - 
            anisotropy_energy);
}

flt measure::get_scalar_average(Lattice3D<Spin> const &lattice, 
                                Vector3 const &vec)
{
    // get dimensions
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
    // sum up all scalar products
    flt scalarAverage = 0;
#pragma omp parallel for reduction(+ : scalarAverage)
    for (int x = 0; x < Lx; x++)
    {
        for (int y = 0; y < Ly; y++)
        {
            for (int z = 0; z < Lz; z++)
            {
                scalarAverage += vec | lattice(x, y, z);
            }
        }
    }
    // normalize the value
    return scalarAverage / (Lx*Ly*Lx);
}

flt measure::get_correlation(Lattice3D<Spin> const &start, 
                                  Lattice3D<Spin> const &next)
{
    // get dimension
    uint Lx = start.Lx();
    uint Ly = start.Ly();
    uint Lz = start.Lz();
    // sum up the correlation between each vetor
    flt correlation = 0;
#pragma omp parallel for reduction(+ : correlation)
    for (int x = 0; x < Lx; x++)
    {
        for (int y = 0; y < Ly; y++)
        {
            for (int z = 0; z < Lz; z++)
            {   
                correlation += start(x, y, z) | next(x, y, z);
            }
        }
    }
    // normalize the values
    return correlation / (Lx * Ly * Lz);
}
