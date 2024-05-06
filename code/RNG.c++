#ifndef __RNG_H__
#define __RNG_H__
/*
    random number generation
*/

//#define RandomDebug

//          --- Random
#include <RNG.h++>
#include <random>


#ifndef RandomDebug
namespace rng
{

    std::mt19937 engine;
    /*

    
    */
    void set_seed(unsigned int seed)
    {
        engine.seed(seed);
    }

    /*
    random number generator for our floats

    / @brief
    / @return uniform random number r ∈ [0,1)
    */
    float rand_f32()
    {
        static std::uniform_real_distribution<float> dist(0.f, 1.f);
        return dist(engine);
    }

    /*
    random number generator flt

    / @brief
    / @return uniform random number r ∈ [0,1)
    */
    double rand_uniform()
    {
        static std::uniform_real_distribution<double> dist(0.0, 1.0);
        //return dist(engine);
        return dist(engine);
    }

        /*
    random number generator flt


    /*
    random number generator flt

    / @brief
    / @return uniform random number r ∈ [0,1]
    */
    double rand_gaussian()
    {
        static std::normal_distribution<double> dist(0, 1);
        return dist(engine);
    }

    int rand_int_range(int a, int b)
    {
        std::uniform_int_distribution<int> dist(a, b - 1);
        return dist(engine);
    }

}

#else       // -- Debug energy

#include <iostream>
using std::cout;
using std::endl;
namespace rng
{

    std::mt19937 engine;
    /*

    
    */
    void set_seed(unsigned int seed)
    {
        engine.seed(seed);
        std::cout << endl << "seed is: " << seed << std::endl;
    }

    /*
    random number generator for our floats

    / @brief
    / @return uniform random number r ∈ [0,1)
    */
    float rand_f32()
    {
        static std::uniform_real_distribution<float> dist(0.f, 1.f);
        float x = dist(engine);
        cout << "<" << x << ">";
        return x;
    }

    /*
    random number generator flt

    / @brief
    / @return uniform random number r ∈ [0,1)
    */
    double rand_uniform()
    {
        static std::uniform_real_distribution<double> dist(0.0, 1.0);
        double x = dist(engine);
        cout << "<" << x << ">";
        return x;
    }

    /*
    random number generator flt

    / @brief
    / @return uniform random number r ∈ [0,1]
    */
    double rand_gaussian()
    {
        static std::normal_distribution<double> dist(0, 1);
        double x = dist(engine);
        cout << "<" << x << ">";
        return x;
    }

    int rand_int_range(int a, int b)
    {
        std::uniform_int_distribution<int> dist(a, b - 1);
        int x = dist(engine);
        cout << "<" << x << ">";
        return x;
    }

}
#endif // RandomDebug
#endif // __RNG_H__