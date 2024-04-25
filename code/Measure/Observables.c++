#include <Measure/Observables.h++>

Vector3 measure::get_magnetization(const Lattice &lattice)
{

    // Define the magnetization as a vector
    // e. g. magnetization[0] = x-component of the vector M
    // The return value magnetization is NOT normalized by the total no. of spins

    Vector3 magnetization = {0, 0, 0};
    for (int x = 0; x < lattice.Lx(); x++)
    {
        for (int y = 0; y < lattice.Ly(); y++)
        {
            for (int z = 0; z < lattice.Lz(); z++)
            {
                magnetization += lattice(x, y, z);
            }
        }
    }
    return magnetization.normalized();
}

F64 measure::get_energy(const Lattice &lattice, Vector3 h_vec, F64 J)
{

    /*
    Define the energy as a scalar (flt)
    given by the Hamiltonian:

    H = -J 𝚺_{<i,j>}{ 𝚺_{α ∈ (x,y,z)} (S_i)_α * (S_j)_α
                    - 𝚺_i 𝚺_{α ∈ (x,y,z)} (h)_α * (S)_α }

    The first term is a summation over all possible bonds (depending
    on the boundary conditions in the lattice, while the second term
    is similar to the magnetization.
    */

    F64 spin_interaction_energy = 0;
    F64 mag_interaction_energy = 0;
    F64 bond_factor = 0;

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

    uint lattice_lx = lattice.Lx();
    uint lattice_ly = lattice.Ly();
    uint lattice_lz = lattice.Lz();

    // sum over all bonds parallel to x-direction
    for (int x = 0; x < lattice_lx - bond_factor; x++)
    {
        for (int y = 0; y < lattice_ly; y++)
        {
            for (int z = 0; z < lattice_lz; z++)
            {
                spin_interaction_energy +=
                    lattice(x, y, z) | lattice(x + 1, y, z);
            }
        }
    }

    // sum over all bonds parallel to y-direction
    for (int x = 0; x < lattice_lx; x++)
    {
        for (int y = 0; y < lattice_ly - bond_factor; y++)
        {
            for (int z = 0; z < lattice_lz; z++)
            {
                spin_interaction_energy +=
                    lattice(x, y, z) | lattice(x, y + 1, z);
            }
        }
    }

    // sum over all bonds parallel to z-direction
    for (int x = 0; x < lattice_lx; x++)
    {
        for (int y = 0; y < lattice_ly; y++)
        {
            for (int z = 0; z < lattice_lz - bond_factor; z++)
            {
                spin_interaction_energy +=
                    lattice(x, y, z) | lattice(x, y, z + 1);
            }
        }
    }

    // sum over spins to calculate the interaction with the magnetic field
    for (int x = 0; x < lattice_lx; x++)
    {
        for (int y = 0; y < lattice_ly; y++)
        {
            for (int z = 0; z < lattice_lz; z++)
            {
                mag_interaction_energy += h_vec | lattice(x, y, z);
            }
        }
    }

    return -J * spin_interaction_energy - mag_interaction_energy;
}
