#include <Ippl.h>
#include <mpi.h>
#include <Heisenberg.h++>
#include "/usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h"

int main(int mainArgCount, char** mainArgs){

    ippl::initialize(mainArgCount, mainArgs, MPI_COMM_WORLD);
    {
    // find out our rank and the number of processors
    int rank = ippl::Comm->rank();
    int numRanks = ippl::Comm->size();
    cout << "Hello from rank " << rank << " of " << numRanks 
            << endl;

    const uint seed = 42;
    Lattice lattice(32,16,16, seed);
    cout << "lattice(1,1,1) = " << lattice(1,1,1) << endl;
    cout << "lattice(0,0,0) = " << lattice(0,0,0) << endl;
    cout << "lattice(-1,0,0) = " << lattice(-1,0,0) << endl;
    cout << "lattice(Lx,Ly,Lz) = " 
         << lattice(lattice.Lx(),lattice.Ly(),lattice.Lz()) << endl;

    cout << "Lz = " << lattice.Lz() << endl;
    cout << "Ly = " << lattice.Ly() << endl;
    cout << "Lx = " << lattice.Lx() << endl;

    ippl::Comm->barrier();}
    ippl::finalize();
}