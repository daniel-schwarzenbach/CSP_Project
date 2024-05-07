#ifndef __DATAHANDLER_H__
#define __DATAHANDLER_H__
/*
    funciton for reading and stroring data
*/

#include <Heisenberg.h++>

namespace data
{
    /*
    reads in a c_string/*char as a f64 floatingpoint
    example use: flt v = read_flt("100.444")
    */
    flt read_flt(char *in);

    /*
    reads in a c_string/*char as a f64 floatingpoint
    example use: int v = read_flt("100")
    */
    int read_int(char *in);

    /*
    creates a folder in the specified path
    */
    bool make_folder(string foldername);

    /*
    stores Arrays in a specified file:<filename>
    */
    bool store_data(const Array<Array<flt>> &data,
                    const string &filename);

    // appends a sting to a line in the file
    bool append_line_in_file(string const &filename,
                             string const &data, u64 const &line);

    // appends a stings to a lines in the file
    bool append_lines_in_file(string const &filename,
                              Array<string> const &datas, 
                              Array<u64> const &lines);

    /*
    loads Array form a specified file:<filename>
    */
    Array2D<flt> load_data(string const &filename);

    /*
    stores the entire lattice in a file that is readable with
    load_lattice
    / @param lattice: the lattice you want to store
    / @param filenam: the file that you want to read in
    / @return returns if the process succeds
    */
    bool store_lattice(Lattice3D<Spin> const &lattice,
                       string const &filename);

    /*
    stores the entire lattice in a file that is readable with
    load_lattice
    */
    bool load_lattice(Lattice3D<Spin> &lattice,
                      string const &filename);
};

#endif // __DATAHANDLER_H__