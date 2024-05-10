/*
This program compares the parallel metorpolis small step with the non
parallelized version, in terms of physical quantaties and speed-up
*/
#include <matplot/matplot.h>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timer.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <Simulations/Simulation.h++>

namespace plt = matplot;

const flt Ns_met = 1e+7;
// end Time
const flt Nmax_met = 1e+9;
// int Random Lattice Seed
const int seed = 69;
// side Lenth
const uint L = 16;
// interaction strenth
const flt J = 1.0;
// no magnetic field
const Spin h = {0,0,0};
// no anisotropy
const Spin k = {0,0,0};

int main(int mainArgCount, char **mainArgs)
{
    // make folders
    data::make_folder("plots");
    data::make_folder("data");
    // init lattice
    Lattice lattice(L, L, L);
    // temperatures
    Array<flt> Ts =
    {0.01, 0.1, 0.3, 0.5, 0.7, 1.0, 1.2, 1.3, 1.4,1.5, 1.7, 2.0,
    3.0, 4.0, 5.0, 10, 100};
    // loop over every temperature
    for (flt T : Ts)
    {
        cout << "running for T = " << T << endl;
        // make foder for the data
        string Tstr = "T_" + to_string(uint(T*1e3)) + "e-3/";
        string Folder = "data/" + Tstr;
        data::make_folder(Folder);

        //              --- small step metropolis omp
        cout << "running small step metropolis omp" << endl;
        rng::set_seed(seed);
        // randomize lattice
        lattice.randomize();
        // reset adaptive variables
        restet_adaptive_omp();
        // test the algorithm
        Array2D<flt> metro_omp = sim::ns::test_algorithm(
                lattice, Ns_met, Nmax_met, T,
            J, h, k, sim::ns::metropolis_smallStep_omp);
        // store the data
        data::store_data(metro_omp,
                        Folder + "/metropolis_omp");

        //              --- small step metropolis
        cout << "running adaptive step metropolis" << endl;
        rng::set_seed(seed);
        // randomize lattice
        lattice.randomize();
        // reset adaptive variables
        restet_adaptive();
        // test the algorithm
        Array2D<flt> metro = sim::ns::test_algorithm(
            lattice, Ns_met, Nmax_met, T,
            J, h, k, sim::ns::metropolis_smallStep);
        // store the data
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
            plt::cla();
        }

        // calculate and cout the parallel speed-up
        flt Speed_up = metro[3][metro[3].size()-1]/
                        metro_omp[3][metro_omp[3].size()-1];
        what_is(Speed_up);
        cout << endl << endl;
    }
    return 0;
}