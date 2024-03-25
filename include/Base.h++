#ifndef BASE_HPP
#define BASE_HPP

//          --- Output
#include <iostream>
#include <iomanip>
#include <cmath>

// output
using std::cout;
using std::cerr;
using std::endl;
using std::cin;
using std::setw;

// math
using std::cos;
using std::sin;

//          --- Types

// standart floatingpoint type
//
// usign a generic float has the advantage that we can easily
// change it.
// also writing flt is shorter than double/float/long double
using flt = float;

#include <functional>
using std::function;
using std::mem_fn;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <array>
using std::array;


//          --- Constants


static constexpr flt π = M_PI; //\pi
static constexpr flt ₂π = 2*π; //\_2\pi

// eulers number 
static constexpr flt 𝑒 = M_E; //\mite

//          --- Functions

// returns random number r ∈ [0,1]
static flt randflt(){
    return (flt)rand()/(flt)RAND_MAX;
}


//          --- Exeptions
#include <exception>

using std::exception;


#endif // BASE_HPP