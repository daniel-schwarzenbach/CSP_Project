#include <gtest/gtest.h>

#include <Wolff/wolff.h++>
#include <Metropolis/metropolis.h++>
#include <Measure/Observables.h++>

const uint seed = 245;
const flt  T = 0.0001;
const flt J = 1.0;
const uint L = 4;
const flt dt = 0.2;
const flt testmag = 0.5;

TEST(Algorithm, test_metropolis_omp){
    rng::set_seed(seed);
    Lattice lattice = Lattice::random_lattice(4,4,4);
    metropolis_omp(
            lattice,T,J,dt,_maxUint_,{0,0,0},{0,0,0},
            MoveType::Addaptive);
    flt mag = measure::get_magnetization(lattice).norm();
    EXPECT_TRUE(mag > testmag);
}

TEST(Algorithm, test_metropolis){
    rng::set_seed(seed);
    Lattice lattice = Lattice::random_lattice(4,4,4);
    adaptive_metropolis(
            lattice,T,J,dt,_maxUint_,{0,0,0},{0,0,0},
            60);
    flt mag = measure::get_magnetization(lattice).norm();
    EXPECT_TRUE(mag > testmag);
}

TEST(Algorithm, test_wolff_omp){
    rng::set_seed(seed);
    Lattice lattice = Lattice::random_lattice(4,4,4);
    wolff_omp(
            lattice,T,J,dt,_maxUint_);
    flt mag = measure::get_magnetization(lattice).norm();
    EXPECT_TRUE(mag > testmag);
}

TEST(Algorithm, test_wolff){
    rng::set_seed(seed);
    Lattice lattice = Lattice::random_lattice(4,4,4);
    wolff(
            lattice,T,J,dt,_maxUint_);
    flt mag = measure::get_magnetization(lattice).norm();
    EXPECT_TRUE(mag > testmag);
}