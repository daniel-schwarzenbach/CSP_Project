#include <Heisenberg.h++>
#include <Ploting/PlotLattice.h++>

int main()
{
    //test_all_colors();
    Lattice lattice = Lattice::random_lattice(10, 10, 10);
    plot_lattice(lattice, "");
    make_folder("test");

    return 0;
}