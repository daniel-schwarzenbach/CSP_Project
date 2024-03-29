#include <Ippl.h>
#include <Base.h++> // for randflt, using std::cout, using std::endl

#include <array>
#include <iostream>
#include <typeinfo>

// set the lattice size
constexpr uint Lx = 16, Ly = 8;

// set the dimension of the problem
constexpr uint dim = 2;

// number of steps
constexpr uint numSteps = 10;




// Lets try to do a simple diffusion problem with periodic BC in 2D
int main(int numMainArgs, char* mainArgs[]) {;
    ippl::initialize(numMainArgs, mainArgs);
    {
    // find out our rank and the number of processors
    int rank = ippl::Comm->rank();
    int numRanks = ippl::Comm->size();
    cout << "Hello from rank " << rank << " of "<< numRanks<< endl;
    
    ippl::Index Ix(Lx), Iy(Ly);
    // 
    ippl::NDIndex<dim> owned(Ix, Iy);

    std::array<bool, dim> isParallel;// Specifies SERIAL, PARALLEL
    isParallel.fill(true);

    ippl::FieldLayout<dim> layout(  MPI_COMM_WORLD, 
                                    owned, 
                                    isParallel);

    double dx                        = 1.0 / double(Lx);
    ippl::Vector<double, dim> hx     = dx;
    ippl::Vector<double, dim> origin = 0;

    using Mesh      = ippl::UniformCartesian<double, dim>;
    using Centering = Mesh::DefaultCentering;
    Mesh mesh(owned, hx, origin);
    typedef ippl::Field<double, dim, Mesh, Centering> Field;



    std::cout << layout << std::endl;
    Field field(mesh, layout, 1);
    field = 1.0;

    const ippl::NDIndex<dim>& localIds  = layout.getLocalNDIndex();
    const int numGhosts                 = field.getNghost();
    typename Field::view_type& view  = field.getView();

    // Set the boundary conditions
    typedef ippl::BConds<Field, dim> BCs;
    BCs bCs;
    for (uint i = 0; i < 2 * dim; ++i) {
        bCs[i] = std::make_shared<ippl::PeriodicFace<Field>>(i);
    }
    bCs.findBCNeighbors(field);


    // assign random values to the lattice
    Kokkos::parallel_for(
            "Assign lattice", 
            field.getFieldRangePolicy(),
            KOKKOS_LAMBDA(const int x, const int y) 
    {
        view(x, y) =    randflt();
        const size_t x₀ = x + localIds[0].first() - numGhosts;
        const size_t y₀ = y + localIds[1].first() - numGhosts;
            cout    << "{ " << x₀ << ", " << y₀ << " } = "<< view(x,y)
                    << " ∈ Core_" << rank << endl;
    });

    // update the ghost cells
    ippl::Comm->barrier(); // wait for all cores to finish
    layout.findNeighbors(numGhosts);
    field.updateLayout(layout, numGhosts);
    bCs.apply(field);
    view = field.getView();
    ippl::Comm->barrier(); // wait for all cores to finish

    // diffusion
    for(uint step = 0; step < numSteps; ++step){
        Kokkos::parallel_for(
                "Assign lattice", 
                field.getFieldRangePolicy(numGhosts),
                KOKKOS_LAMBDA(const int x, const int y) 
        {
            // local to global index conversion
            const size_t x₀ = x + localIds[0].first() - numGhosts;
            const size_t y₀ = y + localIds[1].first() - numGhosts;
            // diffustion, finite difference
            // cout << "{ " << x₀ << ", " << y₀ << " } ∈ Core"
            //      << rank << endl;
            view(x, y) =    (view(x+1, y) + view(x-1, y) + 
                            view(x, y+1) + view(x, y-1)  ) / 4.0;
        });

        ippl::Comm->barrier(); // wait for all cores to finish
        // update the ghost cells
        layout.findNeighbors(numGhosts);
        field.updateLayout(layout, numGhosts);
        bCs.apply(field);

        ippl::Comm->barrier(); // wait for all cores to finish
    }

    cout << INFO << rank << " :finished diffusion" << endl;

    std::string fname =       "field_AllBC_" 
                            + std::to_string(rank) 
                            + ".dat";
    Inform out("Output",fname.c_str(),Inform::OVERWRITE,rank);
    field.write(out);

    ippl::Comm->barrier(); // wait for all cores to finish
    }
    ippl::finalize();
    return 0;
}
