/*
    This test is thesting the implementation for any minor errors
*/

#include <Heisenberg.h++>
#include <gtest/gtest.h>

// aceptable error
constexpr flt _eps_ = 1e-3;

using Lattice = Lattice3D<Spin>;

// are spins different
bool operator!=(Spin const &a, Spin const &b)
{
    if (abs(a.phi() - b.phi()) < _eps_ &&
        abs(a.theta() - b.theta()) < _eps_)
        return false;
    return true;
}

// are spins the same
bool operator==(Spin const &a, Spin const &b)
{
    if (abs(a.phi() - b.phi()) < _eps_ &&
        abs(a.theta() - b.theta()) < _eps_)
        return true;
    return false;
}

// test if lattices are the same
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
                    cout << l1(x, y, z) << " != " <<l2(x, y, z)<<endl;
                    what_is(x);
                    what_is(y);
                    what_is(z);
                    return false;
                }
            }
        }
    }
    return true;
}

// test that lattices for the same seed are actually the same and not
// the same for other lattices
TEST(LatticeTests, RandomGeneration)
{
    // set Lattice size
    uint L = 8;
    // set seed
    rng::set_seed(89);
    Lattice l1 = Lattice::random_lattice(L, L, L);
    Lattice l2 = Lattice::random_lattice(L, L, L);
    // they souldn't be the same since the seed has not been reset
    EXPECT_FALSE(is_the_same(l1, l2));
    // restet seed
    rng::set_seed(89);
    l2 = Lattice::random_lattice(L, L, L);
    // now they shoud be the same
    EXPECT_TRUE(is_the_same(l1, l2));
    rng::set_seed(90);
    Lattice l3 = Lattice::random_lattice(L, L, L);
    EXPECT_FALSE(is_the_same(l1, l3));
    rng::set_seed(80);
    Spin s1 = Spin::get_random();
    rng::set_seed(81);
    Spin s2 = Spin::get_random();
    EXPECT_FALSE(is_the_same(l1, l3));
}

// test periodic boundary condition
TEST(LatticeTests, Lattice_Periodic)
{
    uint L = 8;
    rng::set_seed(89);
    Lattice lattice = Lattice::random_lattice(L, L, L);
    // should work for any {x,y,z} ∈ int^3
    int x = -3, y = 500, z = 302;
    // test neighbor loop
    Array<Index> neighbors = {
        {x + 1, y, z}, {x - 1, y, z}, {x, y + 1, z}, 
        {x, y - 1, z}, {x, y, z + 1}, {x, y, z - 1}};
    for (Index neighbor : neighbors)
    {
        int nx = neighbor[0];
        int ny = neighbor[1];
        int nz = neighbor[2];
        EXPECT_TRUE(lattice(neighbor) == lattice(nx, ny, nz));
        EXPECT_TRUE(abs(nx - x) <= 1);
        EXPECT_TRUE(abs(ny - y) <= 1);
        EXPECT_TRUE(abs(nz - z) <= 1);
        // test assignement
        Spin s = {f32(5), f32(8), f32(3 + nx)};
        s.normalize();
        lattice(neighbor) = s;
        EXPECT_TRUE(lattice(nx, ny, nz) == s);
    }
    // test assignmet
    Spin s = {3, 1, 4};
    s.normalize();
    lattice(x, y, z) = s;
    EXPECT_TRUE(lattice(x, y, z) == s);
    // test periodic properties
    EXPECT_TRUE(lattice(0, 0, L) == lattice(0, 0, 0));
    EXPECT_TRUE(lattice(0, L, 0) == lattice(0, 0, 0));
    EXPECT_TRUE(lattice(L, 0, 0) == lattice(0, 0, 0));
    EXPECT_TRUE(lattice(0, 0, L - 1) == lattice(0, 0, -1));
    EXPECT_TRUE(lattice(0, L - 1, 0) == lattice(0, -1, 0));
    EXPECT_TRUE(lattice(L - 1, 0, 0) == lattice(-1, 0, 0));
}

// test dirichlet/zero boundary condition
TEST(LatticeTests, Lattice_Dirichlet_)
{
    uint L = 8;
    rng::set_seed(89);
    Lattice lattice = Lattice::random_lattice(L, L, L);
    lattice.set_boundary_conditions(BC::Dirichlet);
    Spin zero{0, 0, 0};
    // outlside we always get zero
    EXPECT_TRUE(zero == lattice(0, 0, -1));
    what_is(lattice(0, 0, -1));
    EXPECT_TRUE(zero == lattice(0, -1, 0));
    what_is(lattice(0, -2, -1));
    EXPECT_TRUE(zero == lattice(-1, 0, 0));
    what_is(lattice(-1, 0, 0));
    EXPECT_TRUE(zero == lattice(0, 0, L));
    EXPECT_TRUE(zero == lattice(0, L, 0));
    EXPECT_TRUE(zero == lattice(L, 0, 0));
    EXPECT_FALSE(zero == lattice(0, 0, 0));
    // the outside should be not assingable
    Spin s = {0,0,1};
    lattice(-1,-1,-1) = s;
    EXPECT_FALSE(lattice(-1,-1,-1) == s);
    EXPECT_TRUE(lattice(-1,-1,-1) == zero);
}

// test lattice<bool>
TEST(LatticeTests, Lattice3d_bool_periodic)
{
    uint L = 8;
    rng::set_seed(89);
    Lattice3D<bool> lattice = 
            Lattice3D<bool>::constant_lattice(L, L, L, false);
    int x = -3, y = 500, z = 302;
    Array<Index> neighbors = {
        {x + 1, y, z}, {x - 1, y, z}, {x, y + 1, z}, {x, y - 1, z}, {x, y, z + 1}, {x, y, z - 1}};
    for (Index neighbor : neighbors)
    {
        int nx = neighbor[0];
        int ny = neighbor[1];
        int nz = neighbor[2];
        EXPECT_TRUE(lattice.get(neighbor) == lattice.get(nx, ny, nz));
        EXPECT_TRUE(abs(nx - x) <= 1);
        EXPECT_TRUE(abs(ny - y) <= 1);
        EXPECT_TRUE(abs(nz - z) <= 1);
        bool flag = true;
        lattice.set(neighbor, flag);
        EXPECT_TRUE(lattice.get(nx, ny, nz) == flag);
    }
}

TEST(LatticeTests, Lattice3d_bool_dirichlet)
{
    uint L = 4;
    rng::set_seed(89);
    Lattice3D<bool> lattice = Lattice3D<bool>::constant_lattice(L, L, L, false);
    lattice.set_boundary_conditions(BC::Dirichlet);
    EXPECT_TRUE(lattice.get(0, 0, -1) == true);
    EXPECT_TRUE(lattice.get(0, 0, 4) == true);
    EXPECT_TRUE(lattice.get(1, 1, 1) == false);
    // the outside should be not assingable
    bool s = false;
    lattice.set(-1,-1,-1, s);
    EXPECT_FALSE(lattice.get(-1,-1,-1) == s);
    EXPECT_TRUE(lattice.get(-1,-1,-1) == true);
}

#include <Data/DataHandler.h++>

TEST(LatticeTests, Store_and_Load)
{
    uint L = 8;
    rng::set_seed(89);
    Lattice l1 = Lattice::random_lattice(L, L, L);
    Lattice l2 = Lattice::constant_lattice(L, L, L, {0, 0, 1});
    EXPECT_FALSE(is_the_same(l1, l2));
    data::store_lattice(l1, "test_save_lattice");
    data::load_lattice(l2, "test_save_lattice");
    EXPECT_TRUE(is_the_same(l1, l2));
}