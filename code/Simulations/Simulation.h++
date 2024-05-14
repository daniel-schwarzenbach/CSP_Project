#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__
/*
wraps up the algorithms
preprogramt test algoritms for centralized tests
*/
#include <Base.h++>
#include <Metropolis/metropolis.h++>
#include <Wolff/wolff.h++>
#include <Heisenberg.h++>
#include <Measure/LoadingBar.h++>
#include <Measure/Observables.h++>

namespace sim
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
            void(Lattice3D<Spin> &, flt const &, flt const &,
                 flt const &)>;

        static Algorithm metropolis_smallStep_omp =
            [](Lattice3D<Spin> &lattice, flt const &dt, flt const &T,
               flt const &J)
        {
            metropolis_omp(lattice, T, J, dt,
                           _maxU32_, Spin{0, 0, 0}, Spin{0, 0, 0},
                           MoveType::SmallStep);
        };

        static Algorithm metropolis_adaptive_omp =
            [](Lattice3D<Spin> &lattice, flt const &dt, flt const &T,
               flt const &J)
        {
            metropolis_omp(lattice, T, J, dt,
                           _maxU32_, Spin{0, 0, 0}, Spin{0, 0, 0},
                           MoveType::Addaptive);
        };

        static Algorithm metropolis_random_omp =
            [](Lattice3D<Spin> &lattice, flt const &dt, flt const &T,
               flt const &J)
        {
            metropolis_omp(lattice, T, J, dt,
                           _maxU32_, Spin{0, 0, 0}, Spin{0, 0, 0},
                           MoveType::Random);
        };

        static Algorithm metropolis_adaptive =
            [](Lattice3D<Spin> &lattice, flt const &dt, flt const &T,
               flt const &J)
        {
            adaptive_metropolis(lattice, T, J, dt,
                                _maxU32_, Spin{0, 0, 0}, Spin{0, 0, 0},
                                60);
        };

        static Algorithm metropolis_smallStep =
            [](Lattice3D<Spin> &lattice, flt const &dt, flt const &T,
               flt const &J)
        {
            metropolis(lattice, T, J, dt,
                       _maxU32_, Spin{0, 0, 0}, Spin{0, 0, 0},
                       MoveType::SmallStep);
        };

        static Algorithm metropolis_random =
            [](Lattice3D<Spin> &lattice, flt const &dt, flt const &T,
               flt const &J)
        {
            metropolis(lattice, T, J, dt,
                       _maxU32_, Spin{0, 0, 0}, Spin{0, 0, 0},
                       MoveType::Random);
        };

        static Algorithm wolff_ =
            [](Lattice3D<Spin> &lattice, flt const &dt, flt const &T,
               flt const &J)
        {
            wolff(lattice, T, J, dt, _maxU32_, Spin{0, 0, 0});
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
            Lattice3D<Spin> &lattice,
            flt const &dt, flt const &t_end, flt const &T,
            flt const &J,
            Algorithm const &algorithmus, bool loadingbar = true);

    }

    namespace ns
    {

        /*
        Function to measure after a certain time step

        / @param lattice: lattice to work with
        / @param ns: timestep
        / @param T: temperature
        */
        using Algorithm = function<
            void(Lattice3D<Spin> &, uint const &, flt const &,
                 flt const &,
                 Spin const &, Spin const &)>;

        static Algorithm metropolis_smallStep_omp =
            [](Lattice3D<Spin> &lattice, flt const &ns, flt const &T,
               flt const &J,
               Spin const &h, Spin const &k)
        {
            metropolis_omp(lattice, T, J, _inf_, ns, h, k,
                           MoveType::SmallStep);
        };

        static Algorithm metropolis_adaptive_omp =
            [](Lattice3D<Spin> &lattice, flt const &ns, flt const &T,
               flt const &J,
               Spin const &h, Spin const &k)
        {
            metropolis_omp(lattice, T, J, _inf_, ns,
                           h, k,
                           MoveType::Addaptive);
        };

        static Algorithm metropolis_random_omp =
            [](Lattice3D<Spin> &lattice, flt const &ns,
               flt const &T, flt const &J,
               Spin const &h, Spin const &k)
        {
            metropolis_omp(lattice, T, J, _inf_, ns,
                           h, k,
                           MoveType::Random);
        };

        static Algorithm metropolis_adaptive =
            [](Lattice3D<Spin> &lattice, flt const &ns,
               flt const &T, flt const &J,
               Spin const &h, Spin const &k)
        {
            adaptive_metropolis(lattice, T, J, _inf_,
                                ns, h, k,
                                60);
        };

        static Algorithm metropolis_smallStep =
            [](Lattice3D<Spin> &lattice, flt const &ns,
               flt const &T, flt const &J,
               Spin const &h, Spin const &k)
        {
            metropolis(lattice, T, J, _inf_, ns, h, k,
                       MoveType::SmallStep);
        };

        static Algorithm metropolis_random =
            [](Lattice3D<Spin> &lattice, flt const &ns, flt const &T,
               flt const &J,
               Spin const &h, Spin const &k)
        {
            metropolis(lattice, T, J, _inf_, ns,
                       h, k,
                       MoveType::Random);
        };

        static Algorithm wolff_ =
            [](Lattice3D<Spin> &lattice, flt const &ns, flt const &T,
               flt const &J,
               Spin const &h, Spin const &k)
        {
            wolff(lattice, T, J, _inf_, ns, h);
        };

        /*
        function that runs the algorithm and collects the data after
        ns steps

        @param dt: time step size
        @param numSteps: simulation Steps total
        @param T: temperature
        @param J: interaction Strenth
        @return Array2D<flt>:
        {time, magnetization, M_z, energy} with same size

        */
        Array2D<flt> test_algorithm(
            Lattice3D<Spin> &lattice,
            uint const &ns, u64 const &numSteps, flt const &T,
            flt const &J,
            Spin const &h, Spin const &k,
            Algorithm const &algorithmus,
            bool const &loadingbar = true,
            u64 const& startStep = 0, 
            flt const& startTime = 0.0);

    }
}
#endif // __ALGORITHM_H__