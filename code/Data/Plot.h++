#ifndef __PLOTLATTICE_H__
#define __PLOTLATTICE_H__

#include <Heisenberg.h++>

namespace dat{

bool plot_lattice_slice(Lattice &lattice, int z = 0, 
                        string filename = "");

// plots a quiver for 
bool plot_lattice(  Lattice &lattice, 
                    string filename = "");

string get_filename(string prefix,uint L, F64 J, F64 T, F64 Time);

/*
    @brief: convert_pngs_to_gif
    @param: gifname: name of the gif file
    @param: pngfilePrefix: prefix of the png files
    @details: ImageMagick is required


    @return: true if successful, false otherwise
*/
bool convert_pngs_to_gif(string gifname, string pngfilePrefix);


bool test_all_colors();

}

#endif // __PLOTLATTICE_H__