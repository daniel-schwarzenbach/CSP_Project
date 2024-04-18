#include <Heisenberg.h++>
#include <Ploting/PlotLattice.h++>

int main(){
    Lattice lattice = Lattice::random_lattice(10, 10, 10);
    plot_lattice(lattice, "lattice.png");
    return 0;
}