#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <Base.h++>

std::ostream& operator<<(std::ostream& os, Array<flt> array);

/*
splits up an array for each rank for parrallel execution

/ @brief
/ @param array: 
/ @return sub array for th
*/
Array<flt> split_array_mpi(const Array<flt>& array, int rank,
        int size);

/*
gives a sub range of the array with the desired range

/ @brief
/ @param a: startpoint
/ @param b: endpoint
/ @param array: full array
/ @return {array[a], array[a+1], ..., array[b-1], array[b]}
*/
Array<flt> sub_range(u64 const& a, u64 const& b, 
                    Array<flt> const& array);

/*
calculate the sum of an array

/ @brief
/ @param array: Array of f64
/ @return sum over all elements: ∑ m[i]
*/
flt sum(Array<flt> const &array);

/*
calculate the mean value of a vector

/ @brief
/ @param array: vector of Floats
/ @return mean value of the vector: m = 1/m.size() * ∑ m[i]
*/
template <typename Float>
Float mean(Array<Float> const &array);

/*
calculate the variance of a vector

/ @brief
/ @param array: vector of Floats
/ @return mean value of the vector: m = 1/m.size() * ∑ m[i]
*/
template <typename Float>
Float variance(Array<Float> const &array);

/*
converts t -> u : important for generic convert_array

/ @brief
/ @param t: to convert from
/ @param u: to convert to
*/
template <typename T, typename U>
void convert_types(T const& t, U& u){
    u = t;
}


template <typename T, typename U>
Array<U> convert_array(Array<T> const &array){
    Adress size = array.size();
    Array<U> out(size);
    for (Adress i = 0; i < size; ++i){
        convert_types(array[i], out[i]);
    }
    return out;
}

/*
Push back array
*/
Array<flt>& push_back(Array<flt>& array, 
                        Array<flt> const& to_push_back);

/*
Pushes back each array sub array
*/
Array2D<flt>& push_back(Array2D<flt>& array, 
                        Array2D<flt> const& to_push_back);




// template defitions
extern template f32 mean<f32>(Array<f32> const &);
extern template f64 mean<f64>(Array<f64> const &);
extern template f32 variance<f32>(Array<f32> const &);
extern template f64 variance<f64>(Array<f64> const &);




#endif // __ARRAY_H__