#ifndef __PLOTLATTICE_H__
#define __PLOTLATTICE_H__
/*
    simplified ploting for lattices
*/

#include <Heisenberg.h++>

namespace data
{
    // plots φ of a lattice slice
    bool plot_lattice_slice(Lattice3D<Spin> &lattice, int z = 0,
                            string filename = "");

    // plots a quiver for lattice
    bool plot_lattice(Lattice3D<Spin> &lattice,
                      string filename = "");

    // plots a small quiver for lattice
    bool plot_lattice_small(Lattice3D<Spin> &lattice,
                      string filename = "");

    /*
    convert_pngs_to_gif
    / @param: gifname: name of the gif file
    / @param: pngfilePrefix: prefix of the png files
    / @details: ImageMagick is required
    / @return: true if successful, false otherwise
    */
    bool convert_pngs_to_gif(string gifname, string pngFolder);

}

#endif // __PLOTLATTICE_H__