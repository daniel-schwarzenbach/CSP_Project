#ifndef __BASE_HPP__
#define __BASE_HPP__

//          --- System
// #define WITH_IPPL

//          --- Output
#include <iostream>
#include <iomanip>
#include <string>
using std::string;
using std::to_string;

static const string RESET = "\033[0m";   //   RESET
static const string RED = "\033[31m";    //   RED
static const string GREEN = "\033[32m";  /* Green */
static const string YELLOW = "\033[33m"; /* Yellow */
static const string BLUE = "\033[35m";   /* Blue */

static const string ERROR = "\033[31m [Error]: \033[0m";
static const string WARNING = "\033[35m [Warning]: \033[0m";
static const string INFO = "\033[32m [INFO]: \033[0m";



// output
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::setw;

#ifdef DEBUG
#define what_is(expr)\
    std::cout << #expr << " = " << (expr) << std::endl
#else
#define what_is(expr)
#endif


//          --- Math
#include <cmath>
// math
using std::cos;
using std::min;
using std::sin;

//          --- Types

// 32 bit floating point
using f32 = float;
// 64 bit floating point
using f64 = double;

/*
standart floatingpoint type

its shorter and faster written than double
*/
using flt = double;

// 8 bit unsigned integer
using u8 = unsigned char;
// 16 bit unsigned integer
using u16 = unsigned short;
// 8 bit signed integer
using i8 = signed char;
// Memory Adress
using Adress = size_t;

#include <functional>
using std::function;
using std::mem_fn;

#include <vector>
#include <list>
// dynamic array
template <typename T>
using Array = std::vector<T, std::allocator<T>>; // std::vector<T, std::allocator<T>>;
template <typename T>
using List = std::list<T, std::allocator<T>>;

#include <array>
// static array
template <typename T, Adress I>
using StaticArray = std::array<T, I>;

//          --- Constants

// π
static constexpr flt _pi_ = M_PI;
// 2π
static constexpr flt _2pi_ = 2 * _pi_;
// π/2
static constexpr flt _pi_2_ = _pi_ / 2.;

// ∞
static constexpr flt _inf_ = INFINITY;

// e: eulers number
static constexpr flt _e_ = M_E;

static constexpr uint maxUint = 0xff'ff'ff'ff;

//          --- Functions
#include <RNG.h++>

//          --- Exeptions
#include <exception>

using std::exception;

//         --- Enumerations

// Boundary Conditions
enum BC
{
    _0, // 0 boundry conditions
    Periodic
};

#endif // BASE_HPP