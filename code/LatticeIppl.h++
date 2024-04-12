#include <Base.h++>

#ifndef __LATTICEIPPL_H__
#define __LATTICEIPPL_H__

#include <Base.h++>
#include <Ippl.h>
#include <Spin.h++>

constexpr uint dim = 3;
constexpr uint numGhostCells = 1;

/*
parralel 3D Lattice using ippl
*/
template <class IpplT>
class LatticeIppl
{
private:
    // using IpplT = ippl::Vector<U16, dim>;
    using Mesh = ippl::UniformCartesian<flt, dim>;
    using Field = ippl::Field<IpplT, dim, Mesh, Mesh::DefaultCentering>;
    using BareField = ippl::BareField<IpplT, dim>;
    using BoundryConditions = ippl::BConds<Field, dim>;
    
    const ippl::NDIndex<dim> owned;
    ippl::FieldLayout<dim> layout;
    Mesh mesh;
    Field field;
    ippl::BConds<Field, dim> bConds;

    // ippl::BareField<IpplT, 3U>& bareField;
    typename Field::view_type &view;

public:
    inline uint Lx() const { 
            return layout.getLocalNDIndex()[0].length(); }
    inline uint Ly() const { 
            return layout.getLocalNDIndex()[1].length(); }
    inline uint Lz() const { 
            return layout.getLocalNDIndex()[2].length(); }
    // constructor
    LatticeIppl(uint _Lx, uint _Ly, uint _Lz, uint seed)
        : owned(ippl::Index(_Lx),ippl::Index(_Ly),ippl::Index(_Lz)),
          layout(MPI_COMM_WORLD, 
                 owned,
                 {true, true, true}),
          mesh(owned, 1.0 / double(_Lx), 0),
          field(mesh, layout, 1),
          view(field.getView())
    {
        srand(seed + ippl::Comm->rank());
        // set the boundry conditions
        set_boundary_conditions(BC::Periodic);
        regenerate(seed);
    }
    // destructor
    ~LatticeIppl() = default;
    // acess operator const
    inline IpplT operator()(int x, int y, int z) const
    {
        return view(x + 1, y + 1, z + 1);
    }
    // acess operator
    inline IpplT &operator()(int x, int y, int z)
    {
        return view(x + 1, y + 1, z + 1);
    }
    // changes the boundry conditions
    void set_boundary_conditions(BC bc)
    {
        switch (bc)
        {
        case BC::Periodic:
            for (uint i = 0; i < 2 * dim; ++i)
            {
                bConds[i] = std::make_shared<
                    ippl::PeriodicFace<Field>>(i);
            }
            break;
        case BC::Dirichlet:
            for (uint i = 0; i < 2 * dim; ++i)
            {
                bConds[i] = std::make_shared<
                    ippl::PeriodicFace<Field>>(i);
            }
            field.setFieldBC(bConds);
            bConds.apply(field);
            break;
        default:
            break;
        }
    }
    // mpi comunication to get the ghost cells
    void comunicate_ghost_cells()
    {
        auto &bCs = field.getFieldBC();
        bCs.apply(field);
        // field.accumulateHalo();
        ippl::Comm->barrier();
        field.fillHalo();
        ippl::Comm->barrier();
    }
    // parrallel reduction
    inline void parrallel_for(
        function<void(uint, uint, uint)> const &f)
    {
        Kokkos::parallel_for("lattice", field.getFieldRangePolicy(), f);
    }

    bool regenerate(int seed = 42)
    {
        srand(seed + ippl::Comm->rank());
        for (int x = -1; x < Lz() + 1; ++x)
        {
            for (int y = -1; y < Ly() + 1; ++y)
            {
                for (int z = -1; z < Lz() + 1; ++z)
                {
                    (*this)(x, y, z) = IpplT();
                }
            }
        }
        return true;
    }
};

#include <Spin.h++>
template class LatticeIppl<SpinCartesian>;
template class LatticeIppl<SpinPolar>;

#endif // __LATTICEIPPL_H__
