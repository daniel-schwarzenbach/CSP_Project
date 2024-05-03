#include <matplot/matplot.h>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Algorithm/Algorithm.h++>

namespace plt = matplot;

const flt dt = 1;
// end Time
const flt t_end = 120.0;
// int Random Lattice Seed
const int seed = 69;
// side Lenth
const uint L = 16;
// interaction strenth
const flt J = 1.0;

int main(int mainArgCount, char **mainArgs)
{
    data::make_folder("plots");
    data::make_folder("data");
    Lattice lattice(L, L, L);
    Array<flt> Ts = 
    {0.001, 0.1, 0.3, 0.5, 0.7, 1.0, 1.2, 1.3, 1.4,1.5, 1.7, 2.0,
    3.0, 4.0, 5.0, 10, 100};
    for (flt T : Ts)
    {
        cout << "running for T = " << T << endl;

        //              --- adaptive step metropolis omp
        cout << "running adaptive step metropolis omp" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> metro_omp_adapt = 
                algo::dt::test_algorithm(lattice, dt, t_end, T,
                J, algo::dt::metropolis_adaptive_omp);
        data::store_data(metro_omp_adapt,
                        "data/metro_adapt/T_" + to_str(T) + ".dat");

        //              --- adaptive step metropolis
        cout << "running adaptive step metropolis" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> metro_adapt = 
                algo::dt::test_algorithm(lattice, dt, t_end, T,
                J, algo::dt::metropolis_adaptive);
        data::store_data(metro_adapt,
                            "data/metro_adapt/T_" + to_str(T) + ".dat");

        //              --- small step metropolis
        cout << "running small step metropolis" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> metro_smst = algo::dt::test_algorithm(
                lattice, dt, t_end, T, J, 
                algo::dt::metropolis_smallStep);
        data::store_data(metro_smst,
                            "data/metro_smst/T_" + to_str(T) + ".dat");

        //              --- random step metropolis
        cout << "running random step metropolis" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> metro_rndm = algo::dt::test_algorithm(
            lattice, dt, t_end, T, J,algo::dt::metropolis_random);
        data::store_data(metro_rndm,
                            "data/metro_rndm/T_" + to_str(T) + ".dat");

        //              --- wolff
        cout << "running wolff" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> wolff_data = algo::dt::test_algorithm(
            lattice, dt, t_end, T, J, algo::dt::wolff_);
        data::store_data(wolff_data,
                            "data/wolff/T_" + to_str(T) + ".dat");

        //              --- wolff omp
        cout << "running wolff omp" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> wolff_omp_data = algo::dt::test_algorithm(
            lattice, dt, t_end, T, J, algo::dt::wolff_omp_);
        data::store_data(wolff_data,
                            "data/wolff/T_" + to_str(T) + ".dat");

        //              --- plot data

        // plot magnitisation
        cout << "plot magnitistion" << endl;
        {
            auto p1 = plt::plot(metro_rndm[0], metro_rndm[1], "--s");
            plt::hold(true);
            auto p4 = plt::plot(metro_smst[0], metro_smst[1], "--s");
            auto p2 = plt::plot(metro_adapt[0],metro_adapt[1],"--s");
            auto p3 = plt::plot(wolff_data[0], wolff_data[1], "--s");
            auto p5 = plt::plot(metro_omp_adapt[0],metro_omp_adapt[1],"--s");
            auto p6 = plt::plot(wolff_omp_data[0], wolff_omp_data[1], "--s");

            auto l = plt::legend({"Metropolis Random",
                                  "Metropolis Small Step",
                                  "Adaptive Metropolis",
                                  "Wolff",
                                  "Adaptive Metropolis Omp",
                                  "Wolff Omp"});
            
            l->location(plt::legend::general_alignment::bottomright);
            plt::xlabel("Time in s");
            plt::ylabel("Magnetistation");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots/magnet/T_" + to_str(T) + ".png");
            plt::hold(false);
        }

        // plot energy
        cout << "plot energy" << endl;
        {
            auto p1 = plt::plot(metro_rndm[0], metro_rndm[3], "--s");
            plt::hold(true);
            auto p4 = plt::plot(metro_smst[0], metro_smst[3], "--s");
            auto p2 = plt::plot(metro_adapt[0],metro_adapt[3],"--s");
            auto p3 = plt::plot(wolff_data[0], wolff_data[3], "--s");
            auto p5 = plt::plot(metro_omp_adapt[0],metro_omp_adapt[3],"--s");
            auto p6 = plt::plot(wolff_omp_data[0], wolff_omp_data[3], "--s");

            auto l = plt::legend({"Metropolis Random",
                                  "Metropolis Small Step",
                                  "Adaptive Metropolis",
                                  "Wolff",
                                  "Adaptive Metropolis Omp",
                                  "Wolff Omp"});
            
            l->location(plt::legend::general_alignment::bottomright);
            plt::xlabel("Time in s");
            plt::ylabel("Energy");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots/energy/T_" + to_str(T) + ".png");
            plt::hold(false);
        }
    }
    return 0;
}