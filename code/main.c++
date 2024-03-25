#include "Wolf/wolf.h++"
#include "Metropolis/metropolis.h++"
#include <Base.h++>
#include <matplot/matplot.h>
#include <Spin.h++>
#include <Lattice3d.h++>


/*
Heisenberg: MAINFUNCTION

- no arguments
*/
int main(int mainArgCount, char** mainArgs){

    // Lattice
    Lattice3d<Spin> lattice(16,16,16, 42);

    // metropolis
    cout << "running metropolis" << endl;
    try{
        metropolis(lattice);
    } catch(exception& e){
        cerr    << "metropolis failed!" << endl
                << e.what() << endl;
    }

    // wolf
    cout << "running wolf" << endl;
    try{
        wolf(lattice);
    } catch(exception& e){
        cerr    << "wolf failed!" << endl
                << e.what() << endl;
    }

    return 0;
}