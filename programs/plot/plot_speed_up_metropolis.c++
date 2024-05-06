#include <matplot/matplot.h>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Algorithm/Algorithm.h++>

namespace plt = matplot;

const flt Ns_met = 1e+7;
// end Time
const flt Nmax_met = 1e+9;

const flt Ns_wolff = 1e+2;
// end Time
const flt Nmax_wolff = 1e+3;
// int Random Lattice Seed
const int seed = 69;
// side Lenth
const uint L = 16;
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
    {0.01, 0.1, 0.3, 0.5, 0.7, 1.0, 1.2, 1.3, 1.4 ,1.5, 1.7, 2.0,
    3.0, 4.0, 5.0, 10, 100};
    for (flt T : Ts)
    {
        cout << "running for T = " << T << endl;
        string Tstr = "T_" + to_string(uint(T*1e3)) + "e-3/";
        string Folder = "data/" + Tstr;
        data::make_folder(Folder);

        
        cout << "running small step metropolis omp" << endl;
        
        rng::set_seed(seed);
        lattice.randomize();
        restet_adaptive_omp();
        Array2D<flt> metro_omp = algo::ds::test_algorithm(
                lattice, Ns_met, Nmax_met, T,
            J, h, k, algo::ds::metropolis_smallStep_omp);
        data::store_data(metro_omp,
                        Folder + "/metropolis_omp");

        
        cout << "running small step metropolis" << endl;

        rng::set_seed(seed);
        lattice.randomize();
        restet_adaptive();
        Array2D<flt> metro = algo::ds::test_algorithm(
            lattice, Ns_met, Nmax_met, T,
            J, h, k, algo::ds::metropolis_smallStep);
        data::store_data(metro,
                        Folder + "/metropolis");


         //              --- plot data

        // plot magnitisation
        cout << "plot magnitistion" << endl;
        {
            auto fig = plt::figure(true);
            fig->size(1000, 1000);
            plt::hold(true);
            auto p2 = plt::plot(metro[0],metro[1],"--s");
            auto p5 = plt::plot(metro_omp[0],metro_omp[1],"--s");
            plt::hold(false);
            auto l = plt::legend({
                                  "Metropolis",
                                  "Metropolis Omp"
                                  });
            
            l->location(plt::legend::general_alignment::bottomright);
            plt::xlabel("Number of Steps");
            plt::ylabel("Magnetization");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots/magnet/T_" + to_str(T) + ".png");
            fig.reset();
            plt::figure(false);
            plt::cla();
            
        }

        // plot energy
        cout << "plot energy" << endl;
        {
            auto fig = plt::figure(true);
            fig->size(1000, 1000);
            plt::hold(true);
            auto p2 = plt::plot(metro[0],metro[2],"--s");
            auto p5 = plt::plot(metro_omp[0],metro_omp[2],"--s");
            plt::hold(false);
            auto l = plt::legend({
                                  "Metropolis",
                                  "Metropolis Omp"
                                  });
            
            l->location(plt::legend::general_alignment::topright);
            plt::xlabel("Number of Steps");
            plt::ylabel("Energy");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots/energy/T_" + to_str(T) + ".png");
            plt::hold(false);
            fig.reset();
            plt::figure(false);
            plt::cla();
        }

                // plot number of steps
        cout << "plot number of steps" << endl;
        {
            auto fig = plt::figure(true);
            fig->size(1000, 1000);
            plt::hold(true);
            auto p2 = plt::plot(metro[3],metro[0],"--s");
            auto p5 = plt::plot(metro_omp[3],metro_omp[0],"--s");
            plt::hold(false);
            auto l = plt::legend({
                                  "Metropolis",
                                  "Metropolis Omp"
                                  });
            
            l->location(plt::legend::general_alignment::topright);
            plt::xlabel("Time in s");
            plt::ylabel("Number of Steps");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots/number_of_steps/" + to_str(T) + ".png");
            plt::hold(false);
            fig.reset();
            plt::figure(false);
            plt::cla();
        }
        flt Speed_up = metro[3][metro[3].size()-1]/
                        metro_omp[3][metro_omp[3].size()-1];
        what_is(Speed_up);
        cout << endl << endl;
    }
    return 0;
}