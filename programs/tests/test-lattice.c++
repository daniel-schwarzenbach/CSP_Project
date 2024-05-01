#include <Heisenberg.h++>

bool operator!=(Spin const &a, Spin const &b)
{
    if (abs(a.phi() - b.phi()) < 1e-4 &&
        abs(a.theta() - b.theta()) < 1e-4)
        return false;
    return true;
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

constexpr uint L = 4;

int main()
{
    rng::set_seed(89);
    Lattice l1 = Lattice::random_lattice(L, L, L);
    Lattice l2 = Lattice::random_lattice(L, L, L);
    cout << "should be false = " << is_the_same(l1, l2) << endl
         << endl;

    rng::set_seed(89);
    l2 = Lattice::random_lattice(L, L, L);
    what_is(is_the_same(l1, l2));

    uint Lx = l1.Lx(); uint Ly = l1.Ly(); uint Lz = l1.Lz();
    l1(3,3,3) = Vector3{5,0,2};
    l1(3,3,1) = Vector3{1,0,2};
    l1(3,3,2) = Vector3{1,0,9};
    what_is(l1(3,3,3));
    what_is(l1(3,3,1));
    what_is(l1(3,3,2));

    return 0;
}