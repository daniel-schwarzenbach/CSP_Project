#include <matplot/matplot.h>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Algorithm/Algorithm.h++>

namespace plt = matplot;

const flt Ns_met = 1e+5;
// end Time
const flt Nmax_met = 1e+7;

const flt Ns_wolff = 1e+2;
// end Time
const flt Nmax_wolff = 1e+3;
// int Random Lattice Seed
const int seed = 69;
// side Lenth
const uint L = 64;
// interaction strenth
const flt J = 1.0;
// h
const Spin h = {0,0,0};
// k
const Spin k = {0,0,0};

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
        string Tstr = "T_" + to_string(uint(T*1e3)) + "e-3/";
        string Folder = "data" + Tstr;
        data::make_folder(Folder);

        //              --- adaptive step metropolis omp
        cout << "running adaptive step metropolis omp" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> metro_omp_adapt = algo::ds::test_algorithm(
                lattice, Ns_met, Nmax_met, T,
            J, h, k, algo::ds::metropolis_adaptive_omp);
        data::store_data(metro_omp_adapt,
                        Folder + "metropolis_adaptive_omp");

        //              --- adaptive step metropolis
        cout << "running adaptive step metropolis" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> metro_adapt = algo::ds::test_algorithm(
            lattice, Ns_met, Nmax_met, T,
            J, h, k, algo::ds::metropolis_adaptive);
        data::store_data(metro_adapt,
                        Folder + "metropolis_adaptive");

        //              --- wolff
        cout << "running wolff" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> wolff_data = algo::ds::test_algorithm(lattice, 
                Ns_met, Nmax_met, T, J, h, k,
                algo::ds::metropolis_smallStep);
        data::store_data(wolff_data, Folder + "wolff");

        //              --- wolff omp
        cout << "running wolff omp" << endl;
        rng::set_seed(seed);
        lattice.randomize();
        Array2D<flt> wolff_omp_data = algo::ds::test_algorithm(lattice, 
                Ns_met, Nmax_met, T, J, h, k,
                algo::ds::metropolis_smallStep);
        data::store_data(wolff_data, Folder + "wolff_omp");

        //              --- plot data

        // plot magnitisation
        cout << "plot magnitistion" << endl;
        {
            plt::hold(true);
            auto p2 = plt::plot(metro_adapt[4],metro_adapt[1],"--s");
            auto p3 = plt::plot(wolff_data[4], wolff_data[1], "--s");
            auto p5 = plt::plot(metro_omp_adapt[4],metro_omp_adapt[1],"--s");
            auto p6 = plt::plot(wolff_omp_data[4], wolff_omp_data[1], "--s");
            plt::hold(false);
            auto l = plt::legend({"Adaptive Metropolis",
                                  "Wolff",
                                  "Adaptive Metropolis Omp",
                                  "Wolff Omp"});
            
            l->location(plt::legend::general_alignment::bottomright);
            plt::xlabel("Time in s");
            plt::ylabel("Magnetistation");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots/magnet/T_" + to_str(T) + ".png");
            plt::figure(false);
            
        }

        // plot energy
        cout << "plot energy" << endl;
        {
            plt::hold(true);
            auto p2 = plt::plot(metro_adapt[4],metro_adapt[3],"--s");
            auto p3 = plt::plot(wolff_data[4], wolff_data[3], "--s");
            auto p5 = plt::plot(metro_omp_adapt[4],metro_omp_adapt[3],"--s");
            auto p6 = plt::plot(wolff_omp_data[4], wolff_omp_data[3], "--s");
            plt::hold(false);
            auto l = plt::legend({
                                  "Adaptive Metropolis",
                                  "Wolff",
                                  "Adaptive Metropolis Omp",
                                  "Wolff Omp"});
            
            l->location(plt::legend::general_alignment::topright);
            plt::xlabel("Time in s");
            plt::ylabel("Energy");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots/energy/T_" + to_str(T) + ".png");
            plt::hold(false);
            plt::figure(false);
        }

                // plot number of steps
        cout << "plot number of steps" << endl;
        {
            plt::hold(true);
            auto p2 = plt::plot(metro_adapt[4],metro_adapt[0],"--s");
            auto p3 = plt::plot(wolff_data[4], wolff_data[0], "--s");
            auto p5 = plt::plot(metro_omp_adapt[4],metro_omp_adapt[0],"--s");
            auto p6 = plt::plot(wolff_omp_data[4], wolff_omp_data[0], "--s");
            plt::hold(false);
            auto l = plt::legend({
                                  "Adaptive Metropolis",
                                  "Wolff",
                                  "Adaptive Metropolis Omp",
                                  "Wolff Omp"});
            
            l->location(plt::legend::general_alignment::topright);
            plt::xlabel("Time in s");
            plt::ylabel("Number of Steps");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots/number_of_steps/" + to_str(T) + ".png");
            plt::hold(false);
            plt::figure(false);
        }
    }
    return 0;
}