#ifndef __LATTICEIPPL_H__
#define __LATTICEIPPL_H__

#include <Heisenberg.h++>
#include <Ippl.h>

constexpr uint dim = 3;
constexpr uint numGhostCells = 1;

using VecIppl  = ippl::Vector<u16, dim>;
constexpr u16 _maxU16_ = 0xff'ff;
constexpr flt u16factor = _maxU16_;

static Vector3 from_ippl(VecIppl const& v){
    return Vector3{v[0]/u16factor, v[1]/u16factor, v[2]/u16factor};
}
static VecIppl to_ippl(Vector3 const& v){
    return VecIppl(v[0]*u16factor, v[1]*u16factor, v[2]*u16factor);
}

using mdrange_type = Kokkos::MDRangePolicy<Kokkos::Rank<3>>;
/*
parralel 3D Lattice using ippl
*/
class LatticeIppl
{
private:
    using Mesh=ippl::UniformCartesian<flt, dim>;
    using Field=ippl::Field<VecIppl,dim,Mesh,Mesh::DefaultCentering>;
    using BoundryConditions=ippl::BConds<Field, dim>;

public:
    const ippl::NDIndex<dim> owned;
    ippl::FieldLayout<dim> layout;
    Mesh mesh;
    Field field;
    ippl::BConds<Field, dim> bConds;

    // ippl::VecIppl<VecIppl, 3U>& bareField;
    typename Field::view_type& view;

public:
    inline uint local_Lx() const
    {
        return layout.getLocalNDIndex()[0].length();
    }
    inline uint local_Ly() const
    {
        return layout.getLocalNDIndex()[1].length();
    }
    inline uint local_Lz() const
    {
        return layout.getLocalNDIndex()[2].length();
    }
    // constructor
    LatticeIppl(uint _Lx, uint _Ly, uint _Lz, uint seed)
        : owned(ippl::Index(_Lx), ippl::Index(_Ly), ippl::Index(_Lz)),
          layout(MPI_COMM_WORLD,
                 owned,
                 {true, true, true}),
          mesh(owned, 1.0 / double(_Lx), 0),
          field(mesh, layout, 1), // with one layer of ghost cells
          view(field.getView())
    {
        srand(seed + ippl::Comm->rank());
        // set the boundry conditions
        set_boundary_conditions(BC::Periodic);
        randomize(seed);
    }
    // destructor
    ~LatticeIppl() = default;
    // acess operator const
    inline VecIppl operator()(int x, int y, int z) const
    {
        return field(x+1,y+1,z+1);
    }
    // acess operator
    inline VecIppl &operator()(int x, int y, int z)
    {
        return view(x+1,y+1,z+1);
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
        case BC::_0:
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
        Kokkos::parallel_for("lattice", field.getFieldRangePolicy(),f);
    }

    bool randomize(int seed = 42)
    {  
        Kokkos::parallel_for(
            "Assign field", field.getFieldRangePolicy(),
            KOKKOS_LAMBDA(const int i, const int j, const int k) {
                view(i, j, k) = to_ippl(Spin::get_random());
        });
        return true;
    }
};

#endif // __LATTICEIPPL_H__
