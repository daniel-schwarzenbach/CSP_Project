#include <Wolf/wolf.h++>
#include <Metropolis/metropolis.h++>
#include <Metropolis/adaptive_metropolis.h++>

#include <Base.h++>
#include <matplot/matplot.h>
#include <Spin.h++>
#include <Lattice3d.h++>
#include <Timekeeper.h++>


/*
Heisenberg: MAINFUNCTION

- no arguments
*/
int main(int mainArgCount, char** mainArgs){

    //              --- random seed
    const uint seed = 42;

    //              --- Lattice
    Lattice3d<Spin> lattice(16,16,16, seed); // 50 KB
    cout << "the lattice uses up: " << lattice.memory_size() 
         << " Bytes" << endl
         << "lattice(2,4,17) = " << lattice(2,4,17) << endl << endl;


    //              --- metropolis
    cout << "running metropolis" << endl;
    TimeKeeper timerK;
    try{
        metropolis(lattice);
    } catch(exception& e){
        cerr    << "metropolis failed!" << endl
                << e.what() << endl;
    }
    timerK.stop();
    cout    << "finished metropolis in " << timerK.time() << " sec"
            << endl;


    //              --- adaptive metropolis
    cout << "running adaptive metropolis" << endl;
    timerK.start();
    try{
        lattice.regenerate(seed); // regenerate the lattice
        adaptive_metropolis(lattice);
    } catch(exception& e){
        cerr    << "metropolis failed!" << endl
                << e.what() << endl;
    }
    timerK.stop();
    cout    << "finished adaptiven metropolis in " << timerK.time() 
            << " sec" << endl;


    //               --- wolf
    cout << "running wolf" << endl;
    timerK.start();
    try{
        lattice.regenerate(seed); // regenerate the lattice
        wolf(lattice);
    } catch(exception& e){
        cerr    << "wolf failed!" << endl
                << e.what() << endl;
    }
    timerK.stop();
    cout    << "finished wolf in " << timerK.time() << " sec"
            << endl;

    return 0;
}