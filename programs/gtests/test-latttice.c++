#include <Heisenberg.h++>
#include <gtest/gtest.h>

constexpr flt _eps_ = 1e-7;

bool operator!=(Spin const &a, Spin const &b)
{
    if (abs(a.phi() - b.phi()) < _eps_ &&
        abs(a.theta() - b.theta()) < _eps_)
        return false;
    return true;
}

bool operator==(Spin const &a, Spin const &b)
{
    if (abs(a.phi() - b.phi()) < _eps_ &&
        abs(a.theta() - b.theta()) < _eps_)
        return true;
    return false;
}

bool is_the_same(Lattice const &l1, Lattice const &l2)
{
    uint Lx = l1.Lx();
    uint Ly = l1.Ly();
    uint Lz = l1.Lz();

    for (uint x = 0; x < Lx; ++x)
    {
        for (uint y = 0; y < Ly; ++y)
        {
            for (uint z = 0; z < Lz; ++z)
            {
                if (l1(x, y, z) != l2(x, y, z))
                {
                    cout << l1(x, y, z) << " != " << l2(x, y, z) << endl;
                    return false;
                }
            }
        }
    }
    return true;
}

TEST(LatticeTests, RandomGeneration){
    uint L = 8;
    rng::set_seed(89);
    Lattice l1 = Lattice::random_lattice(L, L, L);
    Lattice l2 = Lattice::random_lattice(L, L, L);
EXPECT_FALSE(is_the_same(l1,l2));
    rng::set_seed(89);
    l2 = Lattice::random_lattice(L, L, L);
EXPECT_TRUE(is_the_same(l1, l2));
}

TEST(LatticeTests, Lattice_Periodic){
    uint L = 8;
    rng::set_seed(89);
    Lattice lattice = Lattice::random_lattice(L, L, L);
    int x = -3, y = 500, z = 302;
    Array<Index> neighbors = {
        {x + 1, y, z}, {x - 1, y, z}, {x, y + 1, z}, {x, y - 1, z}, 
        {x, y, z + 1}, {x, y, z - 1}};
    for (Index neighbor : neighbors)
    {
        int nx = neighbor[0];
        int ny = neighbor[1];
        int nz = neighbor[2];
EXPECT_TRUE(lattice(neighbor) == lattice(nx,ny,nz));
EXPECT_TRUE(abs(nx - x) <= 1);
EXPECT_TRUE(abs(ny - y) <= 1);
EXPECT_TRUE(abs(nz - z) <= 1);
        Spin s = {f32(5), f32(8),f32(3+nx)};
        s.normalize();
        lattice(neighbor) = s;
EXPECT_TRUE(lattice(nx,ny,nz) == s);
    }
    Spin s = {3,1,4};
    s.normalize();
    lattice(x,y,z) = s;
EXPECT_TRUE(lattice(x,y,z) == s);
    EXPECT_TRUE(lattice(0,0,L) == lattice(0,0,0));
    EXPECT_TRUE(lattice(0,L,0) == lattice(0,0,0));
    EXPECT_TRUE(lattice(L,0,0) == lattice(0,0,0));
    EXPECT_TRUE(lattice(0,0,L-1) == lattice(0,0,-1));
    EXPECT_TRUE(lattice(0,L-1,0) == lattice(0,-1,0));
    EXPECT_TRUE(lattice(L-1,0,0) == lattice(-1,0,0));
}

TEST(LatticeTests, Lattice_Dirichlet_){
    uint L = 8;
    rng::set_seed(89);
    Lattice lattice = Lattice::random_lattice(L, L, L);
    lattice.set_boundary_conditions(BC::_0);
    Spin zero{0,0,0};
    EXPECT_TRUE(zero == lattice(0,0,-1));
    EXPECT_TRUE(zero == lattice(0,-1,0));
    EXPECT_TRUE(zero == lattice(-1,0,0));
    EXPECT_TRUE(zero == lattice(0,0,L));
    EXPECT_TRUE(zero == lattice(0,L,0));
    EXPECT_TRUE(zero == lattice(L,0,0));

    EXPECT_FALSE(zero == lattice(0,0,0));
    
}

TEST(LatticeTests, Lattice3d_bool_periodic){
    uint L = 8;
    rng::set_seed(89);
    Lattice3d<bool> lattice 
            = Lattice3d<bool>::constant_lattice(L, L, L, false);
    int x = -3, y = 500, z = 302;
    Array<Index> neighbors = {
        {x + 1, y, z}, {x - 1, y, z}, {x, y + 1, z}, {x, y - 1, z}, 
        {x, y, z + 1}, {x, y, z - 1}};
    for (Index neighbor : neighbors)
    {
        int nx = neighbor[0];
        int ny = neighbor[1];
        int nz = neighbor[2];
EXPECT_TRUE(lattice.get(neighbor) == lattice.get(nx,ny,nz));
EXPECT_TRUE(abs(nx - x) <= 1);
EXPECT_TRUE(abs(ny - y) <= 1);
EXPECT_TRUE(abs(nz - z) <= 1);
        bool flag = true;
        lattice.set(neighbor, flag);
EXPECT_TRUE(lattice.get(nx,ny,nz) == flag);
    }
}

TEST(LatticeTests, Lattice3d_bool_dirichlet){
    uint L = 4;
    rng::set_seed(89);
    Lattice3d<bool> lattice 
            = Lattice3d<bool>::constant_lattice(L, L, L, false);
    lattice.set_boundary_conditions(BC::_0);
EXPECT_TRUE(lattice.get(0,0,-1) == true);
EXPECT_TRUE(lattice.get(0,0,4) == true);
EXPECT_TRUE(lattice.get(1,1,1) == false);
}