#include <Data/MPI_Helper.h++>


Array<flt> data::split_array(
            const Array<flt>& vec, int rank, int size) {
    // Calculate the size of each split
    size_t splitSize = vec.size() / size;
    size_t startIndex = rank * splitSize;
    size_t endIndex = (rank == size - 1) ? vec.size() : (startIndex + splitSize);

    // Handle the case where the vector cannot be evenly split
    if (vec.size() % size != 0 && rank == size - 1) {
        endIndex = vec.size();
    }

    // Create a sub-vector for the current rank
    Array<flt> subVec(vec.begin() + startIndex, vec.begin() + endIndex);
    return subVec;
}