# Coding Rules

## Git Branches

understanding how github works: https://www.youtube.com/watch?v=k6pxsaXAhbI

tipp: print out the git cheat sheet:
https://about.gitlab.com/images/press/git-cheat-sheet.pdf

our branching:

master(Daniel)\
├── Wolf(Constancia)\
├── Metropolis(Justus)\
└── Observables(Mateo)\

#### How to get updates form parent branch:

git pull origin 'parent'

git merge 'parent'


## Naming Convetions

```cpp
// write everything big on data types
class AppleJuice;

// write just the first word small on variables/instances
type_t appleJuice;

// write erything small and with _ as spaces in functions
type_t apple_juice(type_t const& t);
```

in general use:
-   constexpr: as much as possible, evaluates value by preprocessor.
-   const: as much as possible.
-   inline: on smaller functions, avoids function call

alwas have an is-, can- or has- for bools
```cpp
bool isOver9000 = true;
bool isDone = false;
bool hasSolution = false;
```

always comment your functions like this
1. What does the function do
2. describe the argument
3. what does the function return?
4. if it can throw an exeption!

try to always use `const&` in your function arguments! otherwise it will copy the entire data stucture!
```cpp
/*
wolf algorithm for the Heisenber3D model

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception may fail
*/
bool wolf(Lattice3d<Spin> & lattice);
```


## Our Types

write symbols and types that are often into base.h++. do never write:
`using namespace std;` or any other auto inclusion of a namespace.
only use `using std::something` for the things you regulary need to use.

`using flt = float` - our generic floatingpoint: Allows for an easy change in precition\
if you now you need a certain precition dont hesitate to use another type.

`Lattice3d<Spin>` : Our lattice class that works with different boundry conditions and Spin-types. to-doo!

`using std::array`: instead of using pointer! remember: pointers are dangerous, avoid them at any cost! memory leaks are extremly hard to find in larger code bases.

`using std::vector`: dynamic array, great for collectiong data!

`using std::string`: allows for easyer handling of 

## Exeptions

make use of exceptions, or assert!

example:

```cpp
#include <iostream>
using namespace std;

/*
divides a by b = a/b

- a: nenner
- b: zähler
- returns: a/b
- can throw!
*/
double division(int a, int b) {
   if( b == 0 ) {
      throw "Division by zero condition!";
   }
   return (a/b);
}

/*
MainFunction

- no arguments
- returns: 0=Succes, 1=Failure
*/
int main () {
   int x = 50;
   int y = 0;
   double z = 0;
 
   try {
      z = division(x, y);
      cout << z << endl;
   } catch (const char* msg) {
     cerr << msg << endl;
   }

   return 0;
}
```

see: https://www.tutorialspoint.com/cplusplus/cpp_exceptions_handling.htm

for void function, always return a bool if it had succedet!

example:
```cpp
/*
- i :  pointer to allocated memory
returns: if i is not Null instead of void
- can throw
*/
bool increment_by_one(int* i){
    if(i == nullptr){
        throw std::runtime_error("Nullpointer in increment by one");
        return false;
    }
    ++(*i);
    return true;
}
```


try to avoid unnessesary nesting of if statements by negating it:

```cpp
// never do unnesesarry nesting with if statements
// it leads to spaggetti code
int bad(){
    if(isOver9000){
        if(isDone){
            if(hasSolution){
                // ...
                // a lot of code
                // ...
                return true;
            }
            throw std::domain_error("");
        }
        throw std::runtime_error("");
    }
    throw std::logic_error("");
    return false;
}

// try to negate the if statements instead
bool good(){
    if(!isOver9000){
        throw std::logic_error("");
        return false;
    }
    if(!isDone){
        throw std::runtime_error("");
        return false;
    }
    if(!hasSolution){
        throw std::domain_error("");
        return false;
    }
    // ...
    // a lot of code
    // ...
    return true;
}
```

## Ruler lenth and padding

use a ruler of 70 Symbols

|--------------------------------------------------------------------|

extend lines with a double tab

example:
```cpp
flt my_superlong_function(vector<flt> const& x, vector<flt> const& y, 
        const& vector<flt> z)
{
    // ...
    // code
    // ...
}
```
or use a consitent padding
```cpp
flt my_superlong_function(  vector<flt> const& x, 
                            vector<flt> const& y, 
                            const& vector<flt> z){
    // ...
    // code
    // ...
}
```

if you have a lot of nested loops you can just tap it back at some 
point:

```cpp
for(uint i = 0; i < L; ++i){
    for(uint j = 0; j < L; ++j){
        for(uint k = 0; k < L; ++k){
            for(uint l = 0; l < L; ++l){
                for(uint m = 0; m < L; ++m){
for(uint n = 0; n < L; ++n){
    for(uint o = 0; o < L; ++o){
        // ...
        // code
        // ..
    }
}
                }
            }
        }
    }
}
```

## Unicode symbols

### install:

https://marketplace.visualstudio.com/items?itemName=oijaz.unicode-latex 

or just type 'unicode latex' in the extension tab on vscode

set up an keybinding by
1.  ctrl-shift-p
2.  type: 'unicode: insert matsymbol'
3.  click on the settings icon to the right(Configure Keybinding)

![Keybind](.images/SetKeybind.png)

math symbols are allowed and encuraged when they make the code more 
readable!

leave hints on how to type certain symbols
```cpp
//          --- Constants

constexpr flt π = M_PI; //\pi
constexpr flt ₂π = 2*π; //\_2\pi

// eulers number 
flt 𝑒 = M_E; //\mite

```

Do not use any emojis or unknown Symbols!