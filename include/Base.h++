#ifndef BASE_HPP
#define BASE_HPP

//          --- Output
#include <iostream>
#include <iomanip>

// output
using std::cout;
using std::cerr;
using std::endl;
using std::cin;
using std::setw;

//          --- Types

// standart floatingpoint type
//
// usign a generic float has the advantage that we can easily
// change it.
// also writing flt is shorter than double/float/long double
using flt = double;

#include <string>
using std::string;

#include <functional>
using std::function;


//          --- Functions

// returns random number r ∈ [0,1]
static flt randflt(){
    return (flt)rand()/(flt)RAND_MAX;
}

#endif // BASE_HPP