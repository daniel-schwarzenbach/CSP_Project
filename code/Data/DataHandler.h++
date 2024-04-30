#ifndef __DATAHANDLER_H__
#define __DATAHANDLER_H__

#include <Base.h++>

namespace data
{
    flt read_f64(char *in);
    int read_int(char *in);

    bool make_folder(string foldername);
    bool store_data(const Array<Array<flt>> &data,
                    const string &filename);
    
    template<uint I>
    bool store_data(const StaticArray<Array<flt>, I> &data,
                    const string &filename);

    Array<Array<flt>> load_data(string const &filenam);

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
};

#endif // __DATAHANDLER_H__