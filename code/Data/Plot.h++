#ifndef __PLOTLATTICE_H__
#define __PLOTLATTICE_H__
/*
    simplified ploting for lattices

*/

#include <Heisenberg.h++>

namespace data
{

    bool plot_lattice_slice(Lattice &lattice, int z = 0,
                            string filename = "");

    // plots a quiver for
    bool plot_lattice(Lattice &lattice,
                      string filename = "");

    /*
        @brief: convert_pngs_to_gif
        @param: gifname: name of the gif file
        @param: pngfilePrefix: prefix of the png files
        @details: ImageMagick is required


        @return: true if successful, false otherwise
    */
    bool convert_pngs_to_gif(string gifname, string pngfilePrefix);

}

#endif // __PLOTLATTICE_H__