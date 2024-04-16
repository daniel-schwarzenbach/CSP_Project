#include <Wolf/wolf.h++>
#include <Metropolis/metropolis.h++>
#include <Metropolis/adaptive_metropolis.h++>

#include <matplot/matplot.h>
#include <Heisenberg.h++>
#include <Timekeeper.h++>

/*
This is the main file which we run to get all calculations for the project.
*/

/*Define the Boltzmann constant*/
flt kB = 1.38064852 * 10**(-23);

int main(int mainArgCount, char** mainArgs){

    /*
    Definition of thermodynamic beta
    */

    flt beta(flt T) {
        return 1/(kB * T);
    }

    /*
    Code for the Metropolis, Metropolis Advanced, and Wolff algorithms:
    Input: Reference to Lattice& lattice, temperature (T)
    Output: Nothing (Metropolis, Metropolis Advanced), flt susceptibility (Wolff algorithm)
    */
   
    void Metropolis_algorithm(Lattice& lattice, flt T, flt time) {
        // Here, the Metropolis algorithm acts on a given lattice and changes the lattice
    };

    void Metropolis_advanced_algorithm(Lattice& lattice, flt T, flt time) {
        // Here, the Metropolis advanced algorithm acts on a given lattice and changes the lattice
    };

    flt Wolff_algorithm(Lattice& lattice, flt T, flt time) {
        susceptibility = wolf(lattice, T, time) // Here, the Metropolis algorithm acts on a given lattice and changes the lattice
        return susceptibility
    };

    /*
    Define the parameters of the lattice:
    lattice sizes (Lx, Ly, Lz), interaction strength J, boundary coundtions (BC: o for open, p for periodic), 
    temperature (T), boundary coundtions (BC: o for open, p for periodic)
    */
    flt J = 1;
    int Lx = 2;
    int Ly = 2;
    int Lz = 2;
    flt T = 1;
    flt beta = beta(T);
    BC boundaries = o;

    Lattice lattice(Lx,Ly,Lz,boundaries);

    /*
    How long the algorithm runs, 
    */
    flt time = 10;

    cout << lattice.memory_size();

    Metropolis_advanced_algorithm(lattice, T, time);
    
    cout << lattice.memory_size();
    return 0;
}