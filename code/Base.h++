#ifndef BASE_HPP
#define BASE_HPP

//          --- Output
#include <iostream>
#include <iomanip>
#include <string>
using std::string;

static constexpr string RESET   = "\033[0m";    //   RESET
static constexpr string RED     =   "\033[31m";   //   RED
static constexpr string GREEN   =  "\033[32m";    /* Green */
static constexpr string YELLOW  = "\033[33m";     /* Yellow */
static constexpr string BLUE    = "\033[35m";      /* Blue */

static const string ERROR   =  "\033[31m [Error]: \033[0m";
static const string WARNING =  "\033[35m [Warning]: \033[0m";
static const string INFO    =  "\033[32m [INFO]: \033[0m";

// output
using std::cout;
using std::cerr;
using std::endl;
using std::cin;
using std::setw;

//          --- Math
#include <cmath>
// math
using std::cos;
using std::sin;
using std::min;

//          --- Types

// standart floatingpoint type
//
// usign a generic float has the advantage that we can easily
// change it.
// also writing flt is shorter than double/float/long double
using U8 = unsigned char;
using Size = size_t;

using flt = float;

#include <functional>
using std::function;
using std::mem_fn;



#include <vector>
using std::vector;

#include <array>
using std::array;


//          --- Constants


static constexpr flt π = M_PI; //\pi
static constexpr flt ₂π = 2*π; //\_2\pi
static constexpr flt π_2 = π/2.; //\_2\pi

// eulers number 
static constexpr flt 𝑒 = M_E; //\mite

//          --- Functions


/*
random number generator for our floats

/ @brief
/ @return uniform random number r ∈ [0,1]
*/
static flt randflt(){
    return (flt)rand()/(flt)RAND_MAX;
}


//          --- Exeptions
#include <exception>

using std::exception;


#endif // BASE_HPP