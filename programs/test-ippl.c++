#include <Base.h++>
#include <Spin.h++>
#include <mpi.h>
#include <Ippl.h>


constexpr Size dim = 3;
const int Lx = 16, Ly = 16, Lz = 16;
using SpinMesh = ippl::UniformCartesian<float, dim>;
using Lattice = ippl::Field<Spin, dim, SpinMesh, 
                                SpinMesh::DefaultCentering>;

int main(int numMainArgs, char* mainArgs[]) {
    
    // initialize IPPL
    ippl::initialize(numMainArgs, mainArgs, MPI_COMM_WORLD);
    // use MPI to debug the number of processes
    int processId = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    int numProcesses = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    std::cout << "Hello from Process: " << processId << std::endl;
    std::cout << "There are: " << numProcesses << " Processes" 
                << std::endl;
    // set up Lattice
    ippl::Index Xi(Lx), Yi(Ly), Zi(Lz);
    ippl::NDIndex<dim> domain;
    domain[0] = Xi;
    domain[1] = Yi;
    domain[2] = Zi;
    ippl::FieldLayout<dim> layout(  MPI_COMM_WORLD, domain,
                                    {true,true,true},true);
    Spin zeroSpin;
    SpinMesh spinMesh(domain,{1,1,1}, {0,0,0});
    Lattice field(spinMesh,layout, 1);


    ippl::finalize();
    return 0;
}
