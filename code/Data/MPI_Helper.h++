#ifndef __MPI_HELPER_H__
#define __MPI_HELPER_H__

#include <Heisenberg.h++>

namespace data{

Array<flt> split_array(const Array<flt>& vec, int rank, int size);

}

#endif // __MPI_HELPER_H__