#include <Heisenberg.h++>




/*
Definition of thermodynamic beta

/ @param T: temperature in K
*/
flt Beta(flt const& T)
{
    return 1. / (_kB_ * T);
};

#ifndef WITH_OPENMP

uint omp_get_num_threads(){
    return 1;
}

void omp_set_num_threads(int){}

#endif

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
    Float m = mean<Float>(array);
    uint n = array.size();
    double sum = 0;
    #pragma omp parallel for collapse(1) reduction(+: sum)
    for (uint i = 0; i < n; i++)
        sum += pow(array[i] - m, 2);
    return sum / flt(n);
};

template f32 variance<f32>(Array<f32> const&);
template f64 variance<f64>(Array<f64> const&);

/*
- alternative to to_string
- only gives 3 digits after the comma and removes zeros

*/
string to_str(flt const& value){
    string longstr = to_string(value);
    for(uint i = 0; i < 5; ++i){
        uint id = longstr.length() - 1;
        if(longstr[id] == '0'){
            longstr.resize(id);
        } else{
            break;
        }
    }
    return longstr;
}