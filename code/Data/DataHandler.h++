#ifndef __DATAHANDLER_H__
#define __DATAHANDLER_H__

#include <Base.h++>

namespace dat
{
    flt read_f64(char *in);
    int read_int(char *in);

    bool make_folder(string foldername);
    bool store_data(const Array<Array<flt>> &data,
                    const string &filename);
    
    Array<Array<flt>> load_data(string const& filenam);
};

#endif // __DATAHANDLER_H__