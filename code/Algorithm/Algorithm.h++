#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

/*
preprogramt test algoritms for creating standardiesed tests
*/
#include <Base.h++>
#include <Metropolis/metropolis.h++>
#include <Wolff/wolff.h++>
#include <Heisenberg.h++>
#include <Measure/LoadingBar.h++>
#include <Measure/Observables.h++>

namespace algo
{
    namespace dt
    {

        /*
        Function to measure after a certain time step

        / @param lattice: lattice to work with
        / @param dt: timestep
        / @param T: temperature
        */
        using Algorithm = function<
            void(Lattice &, flt const &, flt const &, flt const &)>;

        static Algorithm wolff_omp_ =
        [](Lattice &lattice, flt const &dt, flt const &T,flt const &J)
        {
            wolff_omp(lattice, T, J, dt, _maxUint_);
        };

        static Algorithm metropolis_smallStep_omp =
        [](Lattice &lattice, flt const &dt, flt const &T,flt const &J)
        {
            metropolis_omp(lattice, T, J, dt,
                           _maxUint_, Spin{0, 0, 0}, Spin{0, 0, 0},
                           MoveType::SmallStep);
        };

        static Algorithm metropolis_adaptive_omp =
        [](Lattice &lattice, flt const &dt, flt const &T,flt const &J)
        {
            metropolis_omp(lattice, T, J, dt,
                           _maxUint_, Spin{0, 0, 0}, Spin{0, 0, 0},
                           MoveType::Addaptive);
        };

        static Algorithm metropolis_random_omp =
        [](Lattice &lattice, flt const &dt, flt const &T,flt const &J)
        {
            metropolis_omp(lattice, T, J, dt,
                           _maxUint_, Spin{0, 0, 0}, Spin{0, 0, 0},
                           MoveType::Random);
        };

        static Algorithm metropolis_adaptive =
        [](Lattice &lattice, flt const &dt, flt const &T,flt const &J)
        {
            adaptive_metropolis(lattice, T, J, dt,
                                _maxUint_, Spin{0, 0, 0}, Spin{0, 0, 0},
                                60);
        };

        static Algorithm metropolis_smallStep =
        [](Lattice &lattice, flt const &dt, flt const &T,flt const &J)
        {
            metropolis(lattice, T, J, dt,
                           _maxUint_, Spin{0, 0, 0}, Spin{0, 0, 0},
                           MoveType::SmallStep);
        };

        static Algorithm metropolis_random =
        [](Lattice &lattice, flt const &dt, flt const &T,flt const &J)
        {
            metropolis(lattice, T, J, dt,
                           _maxUint_, Spin{0, 0, 0}, Spin{0, 0, 0},
                           MoveType::Random);
        };

        static Algorithm wolff_ =
        [](Lattice &lattice, flt const &dt, flt const &T, flt const &J)
        {
            wolff(lattice, T, J, dt, _maxUint_, Spin{0,0,0});
        };

        /*
        function that runs the algorithm and collects the datat after
        every time step dt

        @param dt: time step size
        @param t_end: simulation time
        @param T: temperature
        @param J: interaction Strenth
        @return Array2D<flt>:
        {time, magnetization, M_z, energy} with same size

        */
        Array2D<flt> test_algorithm(
            Lattice &lattice,
            flt const &dt, flt const &t_end,flt const &T,flt const &J,
            Algorithm const &algorithmus, bool loadingbar = true);

    }

    namespace ds
    {

        /*
        Function to measure after a certain time step

        / @param lattice: lattice to work with
        / @param ds: timestep
        / @param T: temperature
        */
        using Algorithm = function<
            void(Lattice &, uint const &, flt const &, flt const &,
            Spin const&, Spin const&)>;

        static Algorithm wolff_omp_ =
        [](Lattice &lattice, flt const &ds, flt const &T,flt const &J,
                Spin const& h, Spin const& k)
        {
            wolff_omp(lattice, T, J, _inf_, ds);
        };

        static Algorithm metropolis_smallStep_omp =
        [](Lattice &lattice, flt const &ds, flt const &T,flt const &J,
                Spin const& h, Spin const& k)
        {
            metropolis_omp(lattice, T, J, _inf_, ds, h,k,
                           MoveType::SmallStep);
        };

        static Algorithm metropolis_adaptive_omp =
        [](Lattice &lattice, flt const &ds, flt const &T,flt const &J,
                Spin const& h, Spin const& k)
        {
            metropolis_omp(lattice, T, J, _inf_, ds, 
                            h,k,
                            MoveType::Addaptive);
        };

        static Algorithm metropolis_random_omp =
        [](Lattice &lattice, flt const &ds, flt const &T,flt const &J,
                Spin const& h, Spin const& k)
        {
            metropolis_omp(lattice, T, J, _inf_, ds, 
                            h,k,
                            MoveType::Random);
        };

        static Algorithm metropolis_adaptive =
        [](Lattice &lattice, flt const &ds, flt const &T,flt const &J,
                Spin const& h, Spin const& k)
        {
            adaptive_metropolis(lattice, T, J, _inf_,
                                ds, h,k,
                                60);
        };

        static Algorithm metropolis_smallStep =
        [](Lattice &lattice, flt const &ds, flt const &T,flt const &J,
                Spin const& h, Spin const& k)
        {
            metropolis(lattice, T, J, _inf_, ds, h, k,
                        MoveType::SmallStep);
        };

        static Algorithm metropolis_random =
        [](Lattice &lattice, flt const &ds, flt const &T,flt const &J,
                Spin const& h, Spin const& k)
        {
            metropolis(lattice, T, J, _inf_, ds, 
                            h, k,
                            MoveType::Random);
        };

        static Algorithm wolff_ =
        [](Lattice &lattice, flt const &ds, flt const &T,flt const &J,
                Spin const& h, Spin const& k)
        {
            wolff(lattice, T, J, _inf_, ds,h);
        };

        /*
        function that runs the algorithm and collects the datat after
        every time step dt

        @param dt: time step size
        @param numSteps: simulation Steps total
        @param T: temperature
        @param J: interaction Strenth
        @return Array2D<flt>:
        {time, magnetization, M_z, energy} with same size

        */
        Array2D<flt> test_algorithm(
            Lattice &lattice,
            uint const &ds, u64 const &numSteps,flt const &T,
            flt const &J,
            Spin const& h, Spin const& k,
            Algorithm const &algorithmus, bool loadingbar = true);

    }
}
#endif // __ALGORITHM_H__