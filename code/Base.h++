#ifndef __BASE_HPP__
#define __BASE_HPP__
/*
    sets and includes basic variables and inclueds most important
    files
*/

// ============================= Output ==============================
#include <iostream>
#include <iomanip>

//          --- string
#include <string>
using std::string;
using std::to_string;

// output colors
static const string RESET = "\033[0m";   //   RESET
static const string RED = "\033[31m";    //   RED
static const string GREEN = "\033[32m";  /* Green */
static const string YELLOW = "\033[33m"; /* Yellow */
static const string BLUE = "\033[35m";   /* Blue */

static const string ERROR = "\033[31m [Error]: \033[0m";
static const string WARNING = "\033[35m [Warning]: \033[0m";
static const string INFO = "\033[32m [INFO]: \033[0m";

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::setw;

// output macro for fast and simple outputs
#define what_is(expr) \
    std::cout << #expr << " = " << (expr) << std::endl

// =============================== Math ==============================
#include <cmath>

using std::cos;
using std::exp;
using std::sin;

using std::max;
using std::min;

// ============================== Types ==============================


// 32 bit floating point
using f32 = float;
// 64 bit floating point
using f64 = double;

/*
standart floatingpoint type

its shorter and faster written than double
*/
using flt = double;

//      --- unsingned

// 8 bit unsigned integer
using u8 = unsigned char;
// 16 bit unsigned integer
using u16 = unsigned short;
// 32 bit unsigned integer
using u32 = unsigned int;
// 64 bit unsigned int
using u64 = unsigned long;

//      --- singned

// 8 bit signed integer
using i8 = signed char;
// 16 bit signed integer
using i16 = signed short;
// 32 bit signed integer
using i32 = signed int;
// 64 bit signed int
using i64 = signed long;

// Memory Adress
using Adress = size_t;

//      --- function types
#include <functional>
using std::function;
using std::mem_fn;

// ========================== Containers =============================
#include <vector>
#include <list>

// dynamic array
template <typename T>
using Array = std::vector<T, std::allocator<T>>;

// nested dynamic array 2d
template <typename T>
using Array2D = Array<Array<T>>;

// nested dynamic array 3d
template <typename T>
using Array3D = Array<Array<Array<T>>>;

// linked list
template <typename T>
using List = std::list<T, std::allocator<T>>;

#include <array>
// static array
template <typename T, Adress I>
using StaticArray = std::array<T, I>;

#include <stack>
// stack
template<typename T>
using Stack = std::stack<T>;

// ========================== Constants ==============================

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

static constexpr u32 _maxU32_ = 0xff'ff'ff'ffU;
static constexpr u64 _maxU64_ = 0xff'ff'ff'ff'ff'ff'ff'ffUL;

// =============================== Etc. ==============================

//          --- Functions
#include <RNG.h++>

//          --- Exeptions
#include <exception>
using std::exception;

//         --- Enumerations

// Boundary Conditions
enum BC
{
    Dirichlet, // 0 boundry conditions
    Periodic   //
};


#endif // BASE_HPP