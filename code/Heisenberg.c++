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
// definitions if no omp so that the code still compiles
uint omp_get_num_threads(){
    return 1;
}
// definitions if no omp so that the code still compiles
uint omp_get_thread_num(){
    return 0;
}
// definitions if no omp so that the code still compiles
void omp_set_num_threads(int){}

#endif

/*
- alternative to to_string
- only gives 3 digits after the comma and removes zeros

*/
string to_str(flt const& value){
    // get the longstring: 1.200000
    string longstr = to_string(value);
    // remove all zeros at the end
    for(uint i = 0; i < 5; ++i){
        uint id = longstr.length() - 1;
        if(longstr[id] == '0'){
            longstr.resize(id);
        } else{
            break;
        }
    }
    // return the changed longsting: 1.2
    return longstr;
}