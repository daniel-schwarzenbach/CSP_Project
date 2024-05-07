#include <gtest/gtest.h>

#include <Wolff/wolff.h++>
#include <Metropolis/metropolis.h++>
#include <Measure/Observables.h++>

const uint seed = 245;
const flt  T = 0.0001;
const flt J = 1.0;
const uint L = 4;
const uint Steps_wolff = 2'00;
const uint Steps_met = 20'000;
const flt testmag_wolff = 0.5;
const flt testmag_met = 0.5;

TEST(Algorithm, test_metropolis_omp){
    // set seed
    rng::set_seed(seed);
    // get a random lattice
    Lattice lattice = Lattice::random_lattice(4,4,4);
    // test metropolis omp
    metropolis_omp(
            lattice,T,J,_inf_,Steps_met,{0,0,0},{0,0,0},
            MoveType::Addaptive);
    // get magnetization and compare it
    flt mag = measure::get_magnetization(lattice).norm();
    EXPECT_TRUE(mag > testmag_met);
}

TEST(Algorithm, test_metropolis){
    // get a random lattice
    rng::set_seed(seed);
    Lattice lattice = Lattice::random_lattice(4,4,4);
    // test metropolis
    metropolis(
            lattice,T,J,_inf_,Steps_met,{0,0,0},{0,0,0});
    // get magnetization and compare it
    flt mag = measure::get_magnetization(lattice).norm();
    EXPECT_TRUE(mag > testmag_met);
}

TEST(Algorithm, test_metropolis_adaptive){
    // get a random lattice
    rng::set_seed(seed);
    Lattice lattice = Lattice::random_lattice(4,4,4);
    // test adaptive metropolis
    adaptive_metropolis(
            lattice,T,J,_inf_,Steps_met,{0,0,0},{0,0,0},
            60);
    // get magnetization and compare it
    flt mag = measure::get_magnetization(lattice).norm();
    EXPECT_TRUE(mag > testmag_met);
}

TEST(Algorithm, test_wolff){
    // get a random lattice
    rng::set_seed(seed);
    Lattice lattice = Lattice::random_lattice(4,4,4);
    // test wolff
    wolff(
            lattice,T,J,_inf_,Steps_wolff,{0,0,0});
    // get magnetization and compare it
    flt mag = measure::get_magnetization(lattice).norm();
    what_is(mag);
    EXPECT_TRUE(mag > testmag_wolff);
}