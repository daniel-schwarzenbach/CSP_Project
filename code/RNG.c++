//          --- Random
#include <RNG.h++>
#include <random>
namespace rng
{

    std::mt19937 engine;
    /*


    */
    void set_seed(unsigned int seed)
    {
        engine.seed();
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
    random number generator F64

    / @brief
    / @return uniform random number r ∈ [0,1)
    */
    double rand_uniform()
    {
        static std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(engine);
    }

    /*
    random number generator F64

    / @brief
    / @return uniform random number r ∈ [0,1]
    */
    double rand_gaussion()
    {
        static std::normal_distribution<double> dist(0, 1);
        return dist(engine);
    }

    int rand_int_range(int a,int b){
        std::uniform_int_distribution<int> dist(a,b-1);
        return dist(engine);
    }

}