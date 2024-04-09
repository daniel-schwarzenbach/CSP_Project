#include <Ippl.h>
#include <Base.h++> // for randflt, using std::cout, using std::endl
#include <mpi.h>

#include <array>
#include <iostream>
#include <typeinfo>
#include "/usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h"

// set the lattice size
constexpr uint Lx = 32, Ly = 16;

// set the dimension of the problem
constexpr uint dim = 2;

// number of steps
constexpr uint numSteps = 200;

using Mesh = ippl::UniformCartesian<double, dim>;
using MeshCentering = Mesh::DefaultCentering;
typedef ippl::Field<double, dim, Mesh, MeshCentering> Field;
typedef ippl::BConds<Field, dim> BCs;

void comunicate_ghost_cells(Field &field)
{
    auto &bCs = field.getFieldBC();

    bCs.apply(field);
    //field.accumulateHalo();
    ippl::Comm->barrier();
    field.fillHalo();
    ippl::Comm->barrier();
}

// Lets try to do a simple diffusion problem with periodic BC in 2D
int main(int numMainArgs, char *mainArgs[])
{
    ippl::initialize(numMainArgs, mainArgs);
    {
        // find out our rank and the number of processors
        int rank = ippl::Comm->rank();
        int numRanks = ippl::Comm->size();
        cout << "Hello from rank " << rank << " of " << numRanks 
             << endl;

        ippl::Index Ix(Lx), Iy(Ly);
        //
        ippl::NDIndex<dim> owned(Ix, Iy);

        std::array<bool, dim> isParallel; // Specifies SERIAL,PARALLEL
        isParallel.fill(true);

        ippl::FieldLayout<dim> layout(MPI_COMM_WORLD,
                                      owned,
                                      isParallel);

        double dx = 1.0 / double(Lx);
        ippl::Vector<double, dim> hx = dx;
        ippl::Vector<double, dim> origin = 0;
        Mesh mesh(owned, hx, origin);

        std::cout << layout << std::endl;
        Field field(mesh, layout, 1);
        field = 1.0;
        

        const ippl::NDIndex<dim> &localIds = layout.getLocalNDIndex();
        const int numGhosts = field.getNghost();
        typename Field::view_type &view = field.getView();

        // Set the boundary conditions
        BCs bCs;
        for (uint i = 0; i < 2 * dim; ++i)
        {
            bCs[i] = std::make_shared<ippl::PeriodicFace<Field>>(i);
        }
        field.setFieldBC(bCs);
        bCs.apply(field);
        

        // assign random values to the lattice
        Kokkos::parallel_for(
            "Assign lattice",
            field.getFieldRangePolicy(0),
            KOKKOS_LAMBDA(const int x, const int y) {
                view(x, y) = randflt();
                const size_t x₀ = x + localIds[0].first() - numGhosts;
                const size_t y₀ = y + localIds[1].first() - numGhosts;
                cout << "{ " << x₀ << ", " << y₀ << " } = " 
                     << view(x, y)
                     << " ∈ Core_" << rank << endl;
            });

        comunicate_ghost_cells(field);

        // diffusion
        for (uint step = 0; step < numSteps; ++step)
        {
            Kokkos::parallel_for(
                "Diffusion_Step",
                field.getFieldRangePolicy(),
                KOKKOS_LAMBDA(const int x, const int y) {
                    // local to global index conversion
                    const size_t x₀ = x + localIds[0].first() 
                            - numGhosts;
                    const size_t y₀ = y + localIds[1].first() 
                            - numGhosts;
                    // diffustion, finite difference
                    view(x, y) = (view(x, y) + view(x + 1, y) + 
                                  view(x - 1, y) +
                                  view(x, y + 1) + view(x, y - 1)) /
                                  5.0;
                });
            comunicate_ghost_cells(field);
        }

        cout << WARNING << "Cell(1,1) = " << view(1, 1) << endl;

        cout << INFO << rank << " :finished diffusion" << endl;

        string fname = "field_" + std::to_string(rank) + ".dat";
        Inform out("Output", fname.c_str(), Inform::OVERWRITE, rank);
        field.write(out);

        ippl::Comm->barrier(); // wait for all cores to finish
    }
    ippl::finalize();
    cout << GREEN << "Success" << RESET << endl;
    return 0;
}
