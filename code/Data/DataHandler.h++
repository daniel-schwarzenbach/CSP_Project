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

    template <uint I>
    bool store_data(const StaticArray<Array<flt>, I> &data,
                    const string &filename);

    bool append_line_in_file(string const &filename,
                             string const &data, u64 const &line);

    bool append_lines_in_file(string const &filename,
                              Array<string> const &datas, Array<u64> const &lines);

    bool store_alo_data(const string &filename,
                        string const &algoname,
                        const Array2D<flt> &data,
                        flt const &T, flt const &J,
                        Spin const &h, Spin const &k,
                        u64 const &Ns, u64 const &Nmax,
                        Index const &L);

    bool append_algo_data(const string &filename,
                          const Array2D<flt> &data,
                          flt const &T);

    /*
    loads Array form a specified file:<filename
    */
    Array2D<flt> load_data(string const &filenam);

    extern template bool store_data<2>(
        const StaticArray<Array<flt>, 2> &,
        const string &);
    extern template bool store_data<3>(
        const StaticArray<Array<flt>, 3> &,
        const string &);
    extern template bool store_data<4>(
        const StaticArray<Array<flt>, 4> &,
        const string &);
    extern template bool store_data<5>(
        const StaticArray<Array<flt>, 5> &,
        const string &);

    bool store_lattice(Lattice const &lattice,
                       string const &filename);

    bool load_lattice(Lattice3D<Spin> &lattice,
                      string const &filename);
};

#endif // __DATAHANDLER_H__