/*
This program compares the parallel metorpolis adaptive with the non
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

const flt Ns = 1e+5;
// end Time
const flt Nmax = 1e+7;
// int Random Lattice Seed
const int seed = 69;
// side Lenth
const uint L = 16;
// interaction strenth
const flt J = 1.0;
// no magnetic field
const Spin h = {0, 0, 0};
// no anisotropy
const Spin k = {0, 0, 0};

int main(int mainArgCount, char **mainArgs)
{
    // make folders
    data::make_folder("plots_met");
    data::make_folder("data_met");
    // init lattice
    Lattice lattice(L, L, L);
    // temperatures
        Array<flt> Ts(mainArgCount-1, 0);
    if (mainArgCount > 1)
    {
        try
        {
            for(uint i = 0; i< mainArgCount-1;++i){
                Ts[i] = data::read_flt(mainArgs[i+1]);
            }
        }
        catch (const std::invalid_argument &e)
        {
            cerr << ERROR
                 << "Invalid argument: please enter a valid "
                 << "floating-point number." << endl;
        }
        catch (const std::out_of_range &e)
        {
            cerr << ERROR << "Out of range: the number is too large."
                 << endl;
            return 0;
        }
    }
    else
    {
        cerr << ERROR << "no Arguments given";
        return 0;
    }
    Array<flt> speedUps(0);
    speedUps.reserve(Ts.size());
    // loop over every temperature
    for (flt T : Ts)
    {
        cout << "running for T = " << T << endl;
        // make foder for the data
        string Tstr = "T_" + to_string(uint(T * 1e3)) + "e-3/";
        string Folder = "data_met/" + Tstr;
        data::make_folder(Folder);

        //              --- adaptive step metropolis omp
        cout << "running metropolis omp" << endl;
        rng::set_seed(seed);
        // randomize lattice
        lattice.randomize();
        // reset adaptive variables
        restet_adaptive_omp();
        // test the algorithm
        Array2D<flt> metro_omp = sim::ns::test_algorithm(
            lattice, Ns, Nmax, T,
            J, h, k, sim::ns::metropolis_smallStep_omp);
        // store the data
        data::store_data(metro_omp,
                         Folder + "/metropolis_omp");

        //              --- metropolis
        cout << "running metropolis" << endl;
        rng::set_seed(seed);
        // randomize lattice
        lattice.randomize();
        // reset adaptive variables
        restet_adaptive();
        // test the algorithm
        Array2D<flt> metro = sim::ns::test_algorithm(
            lattice, Ns, Nmax, T,
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
            auto p2 = plt::plot(metro[0], metro[1], "--s");
            auto p5 = plt::plot(metro_omp[0], metro_omp[1], "--s");
            plt::hold(false);
            auto l = plt::legend({"Metropolis",
                                  "Metropolis Omp"});

            l->location(plt::legend::general_alignment::bottomright);
            plt::xlabel("Number of Steps");
            plt::ylabel("Magnetization");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots_met/magnet/T_" + to_str(T) + ".png");
            plt::cla();
        }

        // plot energy
        cout << "plot energy" << endl;
        {
            auto fig = plt::figure(true);
            fig->size(1000, 1000);
            plt::hold(true);
            auto p2 = plt::plot(metro[0], metro[3], "--s");
            auto p5 = plt::plot(metro_omp[0], metro_omp[3], "--s");
            plt::hold(false);
            auto l = plt::legend({"Metropolis",
                                  "Metropolis Omp"});

            l->location(plt::legend::general_alignment::topright);
            plt::xlabel("Number of Steps");
            plt::ylabel("Energy");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots_met/energy/T_" + to_str(T) + ".png");
            plt::cla();
        }

        // plot number of steps
        cout << "plot number of steps" << endl;
        {
            auto fig = plt::figure(true);
            fig->size(1000, 1000);
            plt::hold(true);
            auto p2 = plt::plot(metro[4], metro[0], "--s");
            auto p5 = plt::plot(metro_omp[4], metro_omp[0], "--s");
            plt::hold(false);
            auto l = plt::legend({"Metropolis",
                                  "Metropolis Omp"});

            l->location(plt::legend::general_alignment::topright);
            plt::xlabel("Time in s");
            plt::ylabel("Number of Steps");
            plt::title("T = " + to_str(T) + ", L = " + to_string(L));
            plt::save("plots_met/number_of_steps/" + to_str(T) + ".png");
            plt::cla();
        }

        // calculate and cout the parallel speed-up
        flt Speed_up = metro[4][metro[4].size() - 1] /
                       metro_omp[4][metro_omp[4].size() - 1];
        what_is(Speed_up);
        speedUps.push_back(Speed_up);
        cout << endl;
    }
    what_is(mean(speedUps));
    what_is(variance(speedUps));
    return 0;
}