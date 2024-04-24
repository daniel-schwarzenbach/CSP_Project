#include "Observables.h++"

vector<flt> measure::get_magnetization(const Lattice& lattice){

    //Define the magnetization as a vector
    //e. g. magnetization[0] = x-component of the vector M
    //The return value magnetization is NOT normalized by the total no. of spins

    vector<flt> magnetization(3);
    for (int x = 0; x < lattice.Lx(); x++){
        for (int y = 0; y < lattice.Ly(); y++){
            for (int z = 0; z < lattice.Lz(); z++){
                // magnetization += lattice(x,y,z); can be 

                magnetization[0] += lattice(x,y,z).x();
                magnetization[1] += lattice(x,y,z).y();
                magnetization[2] += lattice(x,y,z).z(); //CHANGE TO magnetization += lattice(x,y,z);
            }
        }
    }
    return magnetization;
}

flt measure::get_energy(const Lattice& lattice){

    /*
    Define the energy as a scalar (flt)
    given by the Hamiltonian:

    H = -J 𝚺_{<i,j>} 𝚺_{α ∈ (x,y,z)} (S_i)_α * (S_j)_α - 𝚺_i 𝚺_{α ∈ (x,y,z)} (h)_α * (S)_α

    The first term is a summation over all possible bonds (depending on the boundary conditions(
    in the lattice, while the second term is similar to the magnetization.
    */

    flt spin_interaction_energy = 0;
    flt mag_interaction_energy = 0;

    /*
    If we have periodic (p) bondary conditions, then bond_factor = 0 and we sum over all possible bonds,
    including the interaction of the spin at (Lx, 1, 1) along the bond parallel to the x-direction
    with the spin at (Lx+1 = 1, 1, 1). 
    If we have open (o) boundary conditions, then bond_factor = 1 and we only sum over all internal bonds.

    The magnetic interaction energy is not affected by the boundary conditions.
    */

    if (lattice.get_boundary_conditions() == o) {
        bond_factor = 0
    } else {
        bond_factor = 1
    }

    float lattice_lx = lattice.Lx();
    float lattice_ly = lattice.Ly();
    float lattice_lz = lattice.Lz();

    //sum over all bonds parallel to x-direction 
    for (int x = 0; x < lattice_lx-bond_factor; x++){
        for (int y = 0; y < lattice_ly; y++){
            for (int z = 0; z < lattice_lz; z++){
                spin_interaction_energy += lattice(x,y,z) | lattice(x+1,y,z);
            }
        }
    }

    //sum over all bonds parallel to y-direction 
    for (int x = 0; x < lattice_lx; x++){
        for (int y = 0; y < lattice_ly-bond_factor; y++){
            for (int z = 0; z < lattice_lz; z++){
                spin_interaction_energy += lattice(x,y,z) | lattice(x,y+1,z);
            }
        }
    }

    //sum over all bonds parallel to z-direction 
    for (int x = 0; x < lattice_lx; x++){
        for (int y = 0; y < lattice_ly; y++){
            for (int z = 0; z < lattice_lz-bond_factor; z++){
                spin_interaction_energy += lattice(x,y,z) | lattice(x,y,z+1);
            }
        }
    }

    //sum over spins to calculate the interaction with the magnetic field
    for (int x = 0; x < lattice_lx; x++){
        for (int y = 0; y < lattice_ly; y++){
            for (int z = 0; z < lattice_lz; z++){
                mag_interaction_energy += h_vector | lattice(x,y,z);
            }
        }
    }

    return -J * spin_interaction_energy - mag_interaction_energy;
}
