#include <Array.h++>

std::ostream& operator<<(std::ostream& os, Array<flt> array){
    os << "{";
    u64 arrayLast = array.size()-1;
    for (uint i = 0; i < arrayLast; ++i){
        os << array[i] << ", ";
    }
    os << array[arrayLast] << "}";
    return os;
}

Array<flt> split_array_mpi(
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

/*
calculate the sum of an array

/ @brief
/ @param array: vector of Floats
/ @return sum over all elements: ∑ m[i]
*/
flt sum(Array<flt> const &array){
    flt sum;
    uint n = array.size();
    #pragma omp parallel for collapse(1) reduction(+: sum)
    for (uint i = 0; i < n; i++)
        sum += array[i];
    return sum;
}

/*
calculate the mean value of a vector

/ @brief
/ @param array: vector of Floats
/ @return mean value of the vector: m = 1/m.size() * ∑ m[i]
*/
template <typename Float>
Float mean(Array<Float> const& array)
{
    Float sum = 0;
    uint n = array.size();
    #pragma omp parallel for collapse(1) reduction(+: sum)
    for (uint i = 0; i < n; i++)
        sum += array[i];
    return sum / n;
};

// compiles mean for 32 and 64 bit floatingpoint
template f32 mean<f32>(Array<f32> const&);
template f64 mean<f64>(Array<f64> const&);

/*
calculate the variance of a vector

/ @brief
/ @param array: vector of Floats
/ @return mean value of the vector: m = 1/m.size() * ∑ m[i]
*/
template <typename Float>
Float variance(Array<Float> const& array)
{
    // get the mean: m
    Float m = mean<Float>(array);
    // sum u the squared difference of elements array[i] and the mean 
    uint n = array.size();
    double sum = 0;
    // parrallelize
    #pragma omp parallel for collapse(1) reduction(+: sum)
    for (uint i = 0; i < n; i++)
        sum += pow(array[i] - m, 2);
    // return the mean average square difference aka variance
    return sum / flt(n);
};

// compiles varianc for 32 and 64 bit floatingpoint
template f32 variance<f32>(Array<f32> const&);
template f64 variance<f64>(Array<f64> const&);



/*
Push back array
*/
Array<flt>& push_back(Array<flt>& array, 
                        Array<flt> const& to_push_back){
    Adress pushSize = to_push_back.size();
    Adress nowSize = array.size();
    array.reserve(nowSize + pushSize);
    for (Adress i = 0; i < pushSize; ++i){
        array.push_back(to_push_back[i]);
    }
    return array;
}

/*
Pushes back each array sub array
*/
Array2D<flt>& push_back(Array2D<flt>& array, 
                        Array2D<flt> const& to_push_back){
    Adress size = array.size();
    if(size != to_push_back.size()){
        cerr << "Not the same number of Culums" << endl;
        return array;
    }
    for(Adress i = 0; i < size; ++i){
        push_back(array[i], to_push_back[i]);
    }
    return array;
}

/*
gives a sub range of the array with the desired range

/ @brief
/ @param a: startpoint
/ @param b: endpoint
/ @param array: full array
/ @return {array[a], array[a+1], ..., array[b-1], array[b]}
*/
Array<flt> sub_range(u64 const& a, u64 const& b, 
                    Array<flt> const& array){
    u64 arraySize = array.size();
    if (a > b  ||  arraySize <= b){
        throw std::runtime_error("a > b  ||  arraySize <= b");
    }
    u64 subSize = b-a;
    Array<flt> subRange(subSize);

    #pragma omp parallel for
    for(u64 i = 0; i < subSize; ++i){
        subRange[i] = array[i+a];
    }
    return subRange;
}