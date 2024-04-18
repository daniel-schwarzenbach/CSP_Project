#include <Ploting/PlotLattice.h++>
#include <matplot/matplot.h>
namespace plt = matplot;

vector<vector<flt>> lattice_slice(Lattice &lattice, uint z = 0){
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    vector<vector<flt>> latticeSlice(Lx, 
                                     vector<flt>(Ly, 0));
    for (uint x = 0; x < Lx; ++x){
        for (uint y = 0; y < Ly; ++y){
            Spin s = lattice(x, y, z);
            latticeSlice[x][y] = s.phi();
        }
    }
    return latticeSlice;
}

bool plot_lattice(Lattice &lattice, std::string filename){
    
    vector<vector<flt>> latticeSlice = lattice_slice(lattice);
    plt::imagesc(latticeSlice);
    plt::show();

    return 0;
}