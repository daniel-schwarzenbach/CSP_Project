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

const flt Ns = 1e+0;
// end Time
const flt Nmax = 1e+4;
// int Random Lattice Seed
const int seed = 55;
// interaction strenth
const flt J = 1.0;
// no magnetic field
const Spin h = {0, 0, 0};
// no anisotropy
const Spin k = {0, 0, 0};

int main(int argc, char **argv)
{
    // set the threads to 12 to avoid wait times
    omp_set_num_threads(12);
    // make folders
    data::make_folder("plots_wolff");
    // temperatures
    Array<flt> Ts =
        {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 
        1.3, 1.4, 
        1.5, 1.6, 1.7, 1.8, 1.9, 2.0};
    Array<uint> Ls = {2, 4, 8, 16, 32, 64};
    u64 rows = Ts.size() * Ls.size();
    Array2D<flt> data(3);
    for (Array<flt> d : data)
    {
        d.reserve(rows);
    }
    // loop over every lattice size
    for (uint L : Ls)
    {
        what_is(L);
        // init lattice
        Lattice lattice(L, L, L);
        // loop over every temperature
        for (flt T : Ts)
        {
            what_is(T);

            //              --- adaptive step metropolis omp
            cout << "running metropolis omp" << endl;
            rng::set_seed(seed);
            // randomize lattice
            lattice.randomize();
            // test the algorithm
            Array2D<flt> metro_omp = sim::ns::test_algorithm(
                lattice, Ns, Nmax, T,
                J, h, k, sim::ns::wolff_omp_);

            //              --- metropolis
            cout << "running metropolis" << endl;
            rng::set_seed(seed);
            // randomize lattice
            lattice.randomize();
            // test the algorithm
            Array2D<flt> metro = sim::ns::test_algorithm(
                lattice, Ns, Nmax, T,
                J, h, k, sim::ns::metropolis_adaptive);

            
            // cut data to the relevant path
            //sim::ns::fit_datas_after_time(metro_omp, metro);

            // plot magnitisation
            cout << "plot magnitistion" << endl;
            {
                auto fig = plt::figure(true);
                fig->size(1000, 1000);
                plt::hold(true);
                auto p2 = plt::semilogx(metro[4], metro[1], "--s");
                auto p5 = plt::semilogx(metro_omp[4], metro_omp[1], "--s");
                plt::hold(false);
                auto l = plt::legend({"Wolff",
                                    "Wolff Omp"});

                l->location(plt::legend::general_alignment::bottomright);
                plt::xlabel("Time in s");
                plt::ylabel("Magnetization");
                plt::title("T = " + to_str(T) + ", L = " + to_string(L));
                plt::save("plots_wolff/magnet/T=" 
                        + to_string(uint(T*100)) + 
                        "L=" + to_string(L) + ".png");
                plt::cla();
            }

            // plot energy
            cout << "plot energy" << endl;
            {
                auto fig = plt::figure(true);
                fig->size(1000, 1000);
                plt::hold(true);
                auto p2 = plt::semilogx(metro[4], metro[3], "--s");
                auto p5 = plt::semilogx(metro_omp[4], metro_omp[3], "--s");
                plt::hold(false);
                auto l = plt::legend({"Wolff",
                                    "Wolff Omp"});

                l->location(plt::legend::general_alignment::topright);
                plt::xlabel("Time in s");
                plt::ylabel("Energy");
            
                plt::title("T = " + to_str(T) + ", L = " + to_string(L));
                plt::save("plots_wolff/energy/T=" + 
                        to_string(uint(T*100)) + 
                        "L=" + to_string(L) + ".png");
                plt::cla();
            }
        }
        
    }
    return 0;
}