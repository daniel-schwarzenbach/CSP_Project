#include <matplot/matplot.h>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Algorithm/Algorithm.h++>

namespace plt = matplot;

const flt dt = 0.1;
// end Time
const flt t_end = 10.0;
// int Random Lattice Seed
const int seed = 69;
// side Lenth
const uint L = 8;

const flt J = 1.0;

int main(int mainArgCount, char **mainArgs)
{
    data::make_folder("plots");
    data::make_folder("data");
    Lattice lattice(L, L, L);
    for (flt T = 0.1; T <= 10; T += 0.1)
    {

        //              --- adaptive step metropolis
        cout << "running adaptive step metropolis" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> metro_adapt = 
                algo::dt::test_algorithm(lattice, dt, t_end, T,
                J, algo::dt::metropolis_adaptive_omp);
        data::store_data(metro_adapt,
                            "data/metro_adapt_" + to_str(T) + ".dat");

        //              --- small step metropolis
        cout << "running small step metropolis" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> metro_smst = algo::dt::test_algorithm(
                lattice, dt, t_end, T, J, 
                algo::dt::metropolis_smallStep_omp);
        data::store_data(metro_smst,
                            "data/metro_smst_" + to_str(T) + ".dat");

        //              --- random step metropolis
        cout << "running random step metropolis" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> metro_rndm = algo::dt::test_algorithm(
            lattice, dt, t_end, T, J,algo::dt::metropolis_random_omp);
        data::store_data(metro_rndm,
                            "data/metro_rndm_" + to_str(T) + ".dat");

        //              --- wolff
        cout << "running random step metropolis" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> wolff_data = algo::dt::test_algorithm(
            lattice, dt, t_end, T, J, algo::dt::wolff_omp_);
        data::store_data(wolff_data,
                            "data/wolff_" + to_str(T) + ".dat");

        //              --- plot data

        // plot magnitisation
        cout << "plot magnitisation" << endl;
        {
            auto p1 = plt::plot(metro_rndm[0], metro_rndm[1], "--gs");
            plt::hold(true);
            auto p4 = plt::plot(metro_smst[0], metro_smst[1], "--ys");
            auto p2 = plt::plot(metro_adapt[0], metro_adapt[1], "--rs");
            auto p3 = plt::plot(wolff_data[0], wolff_data[1], "--bs");

            auto l = plt::legend({"Metropolis Random",
                                  "Metropolis Small Step",
                                  "Adaptive Metropolis",
                                  "Wolff"});
            l->location(plt::legend::general_alignment::bottomright);
            plt::xlabel("Time in s");
            plt::ylabel("Magnitisation");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots/mag_" + to_str(T) + ".png");
            plt::hold(false);
        }

        // plot aut
        cout << "plot autocorrelation" << endl;
        {
            auto p1 = plt::plot(metro_rndm[0], metro_rndm[3], "--gs");
            plt::hold(true);
            auto p4 = plt::plot(metro_smst[0], metro_smst[3], "--ys");
            auto p2 = plt::plot(metro_adapt[0], metro_adapt[3], "--rs");
            auto p3 = plt::plot(wolff_data[0], wolff_data[3], "--bs");

            auto l = plt::legend({"Metropolis Random",
                                  "Metropolis Small Step",
                                  "Adaptive Metropolis",
                                  "Wolff"});
            l->location(plt::legend::general_alignment::topright);
            plt::xlabel("Time in s");
            plt::ylabel("AutoCorrelation");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots/aut_" + to_str(T) + ".png");
            plt::hold(false);
        }

        // plot energy
        cout << "plot energy" << endl;
        {
            auto p1 = plt::plot(metro_rndm[0], metro_rndm[2], "--gs");
            plt::hold(true);
            auto p4 = plt::plot(metro_smst[0], metro_smst[2], "--ys");
            auto p2 = plt::plot(metro_adapt[0], metro_adapt[2], "--rs");
            auto p3 = plt::plot(wolff_data[0], wolff_data[2], "--bs");

            auto l = plt::legend({"Metropolis Random",
                                  "Metropolis Small Step",
                                  "Adaptive Metropolis",
                                  "Wolff"});
            l->location(plt::legend::general_alignment::bottomright);
            plt::xlabel("Time in s");
            plt::ylabel("Energy");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots/energy_" + to_str(T) + ".png");
            plt::hold(false);
        }
    }
    return 0;
}