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
    random number generator flt

    / @brief
    / @return uniform random number r ∈ [0,1)
    */
    double rand_uniform();

    /*
    random number generator flt

    / @brief
    / @return uniform random number r ∈ [-1,1)
    */
    double rand_uniform_singed();

    /*
    random number generator flt

    / @brief
    / @return
    */
    double rand_gaussian();

    /*
    random number generator int

    / @brief
    / @return r ∈ {a, a+1, ..., b-2, b-1}
    */
    int rand_int_range(int a, int b);

    template <class T>
    static T get_random() { return T(100 * rand_uniform()); }

}
#endif // __RNG_H__