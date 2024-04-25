#include <Wolf/wolf.h++>
#include <Metropolis/metropolis.h++>
#include <Metropolis/adaptive_metropolis.h++>

#include <matplot/matplot.h>
#include <Heisenberg.h++>
#include <Timekeeper.h++>

// temperature
flt T = 0.1;
flt J = 1.0;
flt Time = 20.0;
/*
Heisenberg: MAINFUNCTION

- no arguments
*/
int main(int mainArgCount, char **mainArgs)
{

    //              --- random seed
    const uint seed = 42;

    //              --- Lattice
    Lattice lattice = Lattice::random_lattice(16, 16, 16, seed);
    cout << "the lattice uses up: " << lattice.memory_size()
         << " Bytes" << endl
         << "lattice(2,4,17) = " << lattice(2, 4, 17) << endl
         << endl;

    //              --- metropolis
    cout << "running metropolis ..." << endl;
    TimeKeeper timerK;
    try
    {
        metropolis(lattice, T, 10, 100);
    }
    catch (exception &e)
    {
        cerr << ERROR << "metropolis failed!" << endl
             << e.what() << endl;
    }
    timerK.stop();
    cout << INFO << "finished metropolis in " << timerK.time()
         << " sec" << endl;

    cout << "recompile" << endl;

    //              --- adaptive metropolis
    cout << "running adaptive metropolis ..." << endl;
    timerK.start();
    try
    {
        lattice.randomize(seed); // randomize the lattice
        adaptive_metropolis(lattice, T, J, Time);
    }
    catch (exception &e)
    {
        cerr << ERROR << "adaptive metropolis failed!" << endl
             << e.what() << endl;
    }
    timerK.stop();
    cout << INFO << "finishe adaptiven metropolis in "
         << timerK.time() << " sec" << endl;

    //               --- wolf
    cout << "running wolf ..." << endl;
    timerK.start();
    try
    {
        lattice.randomize(seed); // randomize the lattice
        wolf(lattice, T, Time);
    }
    catch (exception &e)
    {
        cerr << ERROR << "wolf failed!" << endl
             << e.what() << endl;
    }
    timerK.stop();
    cout << INFO << "finished wolf in " << timerK.time() << " sec"
         << endl;

    return 0;
}