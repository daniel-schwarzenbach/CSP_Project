#ifndef __PLOTLATTICE_H__
#define __PLOTLATTICE_H__

#include <Heisenberg.h++>

static Array<Array<double>> rainbow_dark {
    {0.0, 0.0, 0.9},
    {0.0, 0.6, 0.6},
    {0.0, 0.9, 0.0},
    {0.6, 0.6, 0.0},
    {0.9, 0.0, 0.0},
    {0.6, 0.0, 0.6},
    {0.0, 0.0, 0.9}
};

bool plot_lattice_slice(Lattice &lattice, int z = 0, 
                        string filename = "");

// plots a quiver for 
bool plot_lattice(  Lattice &lattice, 
                    string filename = "");


/*
    @brief: convert_pngs_to_gif
    @param: gifname: name of the gif file
    @param: pngfilePrefix: prefix of the png files
    @details: ImageMagick is required


    @return: true if successful, false otherwise
*/
bool convert_pngs_to_gif(string gifname, string pngfilePrefix);

bool make_folder(string foldername);

bool test_all_colors();

#endif // __PLOTLATTICE_H__