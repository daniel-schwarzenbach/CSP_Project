#include <Measure/Observables.h++>

#include <numeric>
#include <algorithm>

Vector3 measure::get_magnetization(const Lattice &lattice)
{

    // Define the magnetization as a vector
    // e. g. magnetization[0] = x-component of the vector M
    // The return value magnetization is NOT normalized by the total no. of spins
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
    f32 sx = 0;
    f32 sy = 0;
    f32 sz = 0;
#pragma omp parallel for reduction(+ : sx, sy, sz)
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
    return Vector3{sx, sy, sz} / (Lx * Ly * Lz);
}

flt measure::get_energy(const Lattice &lattice, Vector3 h_vec, flt J)
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

    flt spin_interaction_energy = 0;
    flt mag_interaction_energy = 0;
    flt bond_factor = 0;

    /*
    If we have periodic (p) bondary conditions, then bond_factor = 0 and we sum over all possible bonds,
    including the interaction of the spin at (Lx, 1, 1) along the bond parallel to the x-direction
    with the spin at (Lx+1 = 1, 1, 1).
    If we have open (o) boundary conditions, then bond_factor = 1 and we only sum over all internal bonds.

    The magnetic interaction energy is not affected by the boundary conditions.
    */
    switch (lattice.get_boundary_conditions())
    {
    case BC::_0:
        bond_factor = 1;
        break;
    case BC::Periodic:
        bond_factor = 0;
        break;
    default:
        bond_factor = 0;
        break;
    }

    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
    uint sizeX = Lx - bond_factor;
    uint sizeY = Ly - bond_factor;
    uint sizeZ = Lz - bond_factor;
// sum over all bonds parallel to x-direction
#pragma omp parallel for reduction(+ : spin_interaction_energy)
    for (int x = 0; x < sizeX; x++)
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
        for (int y = 0; y < sizeY; y++)
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
            for (int z = 0; z < sizeZ; z++)
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

    return -J * spin_interaction_energy - mag_interaction_energy
            / (Lx*Ly*Lz);
}

flt measure::get_scalar_average(Lattice const &lattice, Vector3 const &vec)
{
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
    flt scalarAverage;
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
    return scalarAverage;
}

flt measure::get_auto_correlation(Lattice const &prev, Lattice const &next)
{
    uint Lx = prev.Lx();
    uint Ly = prev.Ly();
    uint Lz = prev.Lz();
    flt correlation = 0;
#pragma omp parallel for reduction(+ : correlation)
    for (int x = 0; x < Lx; x++)
    {
        for (int y = 0; y < Ly; y++)
        {
            for (int z = 0; z < Lz; z++)
            {
                correlation += prev(x, y, z) | next(x, y, z);
            }
        }
    }
    return correlation / (Lx * Ly * Lz);
}

// exp(t-t₀ / τ): autocorecaltion time
flt calculate_auto_correlation_time(Array<flt> const &y,
                                    Array<flt> const &t)
{
    flt t0 = t[0];
    flt min_energy = std::numeric_limits<flt>::max();

    // Define the objective function to minimize
    auto objective_function = [y, t, t0](flt tau)
    {
        flt sum = 0.0;
#pragma omp parrallel for
        for (size_t i = 0; i < y.size(); ++i)
        {
            flt diff = y[i] - std::exp((t[i] - t0) / tau);
            sum += diff;
        }
        return sum;
    };

    // Minimization process (simplified example, use a proper minimization algorithm)
    flt maxTau = 1'000'000;
    flt minTau = 0.000'1;
    flt midTau = (maxTau + minTau) / 2.0;
    for (uint i = 0; i < 100; ++i)
    {
        flt diff = objective_function(midTau);
        if (diff < 0)
        {
            minTau = midTau;
        }
        else if (diff > 0)
        {
            maxTau = midTau;
        }
        else if (diff == 0)
        {
            return midTau;
        }
        midTau = (maxTau + minTau) / 2.0;
    }
    // Return the value of τ that minimizes the energy
    return midTau;
}
