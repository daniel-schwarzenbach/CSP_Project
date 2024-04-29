#ifndef __DATAHANDLER_H__
#define __DATAHANDLER_H__

#include <Base.h++>

namespace dat{
    F64 read_f64(char* in);
    F64 read_int(char* in);


    bool make_folder(string foldername);
    bool store_data(const Array<Array<F64>>& data, 
                    const std::string& filename);
};




#endif // __DATAHANDLER_H__