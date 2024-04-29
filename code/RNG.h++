#ifndef __RNG_H__
#define __RNG_H__

//          --- Random
namespace rng
{
    /*
    restets the seed of the random engine

    / @brief
    / @return uniform random number r ∈ [0,1]
    */
    void set_seed(unsigned int seed);

    /*
    random number generator for our floats

    / @brief
    / @return uniform random number r ∈ [0,1]
    */
    float rand_f32();

    /*
    random number generator F64

    / @brief
    / @return uniform random number r ∈ [0,1]
    */
    double rand_f64();

    /*
    random number generator F64

    / @brief
    / @return uniform random number r ∈ [0,1]
    */
    double rand_gaussian();


    template<class T>
    static T get_random(){return T(100*rand_f64());}

}
#endif // __RNG_H__