#include <Metropolis/adaptive_metropolis.h++>
#include <Metropolis/metropolis.h++>
#include <Wolff/wolff.h++>
#include <Data/Plot.h++>
#include <Heisenberg.h++>
#include <Measure/Timekeeper.h++>
#include <Measure/Observables.h++>
#include <Data/DataHandler.h++>
#include <matplot/matplot.h>
#include <LoadingBar.h++>

namespace plt = matplot;

// interaction
const F64 J = 1.0;
// maximal runtime
const F64 dt = 0.1;
// end Time
const F64 t_end = 30.0;
// Temerature
const F64 T = 1.0;
// int Random Lattice Seed
const int seed = 69; // hi hi

// side Lenth
const uint L = 16;

int main(int mainArgCount, char **mainArgs)
{
    // Take input as temperature
    // if (mainArgCount >= 1){
    //     T = dat::read_f64(mainArgs[0]);
    // }
    // if (mainArgCount >= 2){
    //     seed = dat::read_f64(mainArgs[1]);
    // }
    //              --- make folders
    dat::make_folder("plots");
    dat::make_folder("data");

    //              --- init Lattice
    rng::set_seed(seed);
    Lattice lattice = Lattice::random_lattice(L, L, L);
    rng::set_seed(seed);
    lattice.randomize();
    Lattice lcopy = lattice;

    uint size = t_end / dt;
    Array<F64> nm_time(0);
    nm_time.reserve(size);
    Array<F64> nm_mag(0);
    nm_mag.reserve(size);
    Array<F64> nm_nrg(0);
    nm_nrg.reserve(size);
    Array<F64> nm_aut(0);
    nm_aut.reserve(size);

    Array<F64> am_time(0);
    am_time.reserve(size);
    Array<F64> am_mag(0);
    am_mag.reserve(size);
    Array<F64> am_nrg(0);
    am_nrg.reserve(size);
    Array<F64> am_aut(0);
    am_aut.reserve(size);

    Array<F64> wo_time(0);
    wo_time.reserve(size);
    Array<F64> wo_mag(0);
    wo_mag.reserve(size);
    Array<F64> wo_nrg(0);
    wo_nrg.reserve(size);
    Array<F64> wo_aut(0);
    wo_aut.reserve(size);

    dat::plot_lattice(lattice, "aMet.png");
    #pragma omp parallel
    {
        string str =    "hello throm thread" 
                        + to_string(omp_get_thread_num());
    #pragma omp critical
        cout << str << endl;
    } cout << endl;
    //             --- metropolis
    cout << "running metropolis ..." << endl;
    {
        LoadingBar lbar(60);
        // not zero
        F64 t_elapsed = dt;
        int i = 0;
        nm_aut.push_back(1);
        while (t_elapsed < t_end)
        {
            lbar.update(t_elapsed / t_end * 100.0);
            // dat::plot_lattice(
            //         lattice,
            //         "plots/addMetro/metro_" + to_string(i++) + ".png");

            nm_time.push_back(t_elapsed);
            Vector3 magn = measure::get_magnetization(lattice);
            nm_mag.push_back(magn.norm());
            nm_nrg.push_back(abs(measure::get_energy(lattice)));
            TimeKeeper timer;
            timer.start();
            metropolis(lattice, T, J, dt, maxUint, MoveType::SmallStep);
            timer.stop();
            t_elapsed += timer.time();
            F64 aut = measure::get_auto_correlation(lcopy,lattice);
            nm_aut.push_back(aut);
        }

        nm_time.push_back(t_elapsed);
        Vector3 magn = measure::get_magnetization(lattice);
        nm_mag.push_back(magn.norm());
        nm_nrg.push_back(abs(measure::get_energy(lattice)));
        // dat::plot_lattice(
        //             lattice,
        //             "plots/addMetro/metro_" + to_string(i++) + ".png");
    }
    dat::plot_lattice(lattice, "bMet.png");

    // save data
    cout << "Save data" << endl;
    Array<Array<F64>> data_nm = {nm_time, nm_mag, nm_nrg};
    dat::store_data(data_nm, "data/metr.dat");

    rng::set_seed(seed);
    lattice.randomize();

    //             --- adaptive-metropolis
    cout << "running addaptive metropolis ..." << endl;
    {
        am_aut.push_back(1);
        LoadingBar lbar(60);
        // not zero
        F64 t_elapsed = dt;
        int i = 0;
        while (t_elapsed < t_end)
        {
            lbar.update(t_elapsed / t_end * 100.0);
            // dat::plot_lattice(
            //         lattice,
            //         "plots/addMetro/metro_" + to_string(i++) + ".png");

            am_time.push_back(t_elapsed);
            Vector3 magn = measure::get_magnetization(lattice);
            am_mag.push_back(magn.norm());
            am_nrg.push_back(abs(measure::get_energy(lattice)));
            TimeKeeper timer;
            
            timer.start();
            metropolis(lattice, T, J, dt, maxUint, MoveType::Addaptive);
            timer.stop();
            t_elapsed += timer.time();

            F64 aut = measure::get_auto_correlation(lcopy,lattice);
            am_aut.push_back(aut);
        }

        am_time.push_back(t_elapsed);
        Vector3 magn = measure::get_magnetization(lattice);
        am_mag.push_back(magn.norm());
        am_nrg.push_back(abs(measure::get_energy(lattice)));
        // dat::plot_lattice(
        //             lattice,
        //             "plots/addMetro/metro_" + to_string(i++) + ".png");
    }

    // save data
    cout << "Save data" << endl;
    Array<Array<F64>> data_am = {am_time, am_mag, am_nrg};
    dat::store_data(data_am, "data/addmetr.dat");

    //              --- randomiz Lattice
    rng::set_seed(seed);
    lattice.randomize();

    //             --- wolff
    cout << "running wolff ..." << endl;
    {
        wo_aut.push_back(1);
        LoadingBar lbar(60);
        F64 t_elapsed = dt;
        int i = 0;
        while (t_elapsed < t_end)
        {
            lbar.update(t_elapsed / t_end * 100.0);
            // dat::plot_lattice(
            //         lattice,
            //         "plots/wolff/wolff_" + to_string(i++) + ".png");
            wo_time.push_back(t_elapsed);
            Vector3 magn = measure::get_magnetization(lattice);
            wo_mag.push_back(magn.norm());
            wo_nrg.push_back(abs(measure::get_energy(lattice)));
            TimeKeeper timer;
            timer.start();
            wolff(lattice, T, J, dt, maxUint);
            timer.stop();
            t_elapsed += timer.time();

            F64 aut = measure::get_auto_correlation(lcopy,lattice);
            wo_aut.push_back(aut);
        }
        wo_time.push_back(t_elapsed);
        Vector3 magn = measure::get_magnetization(lattice);
        wo_mag.push_back(magn.norm());
        wo_nrg.push_back(abs(measure::get_energy(lattice)));
        // dat::plot_lattice(
        //             lattice,
        //             "plots/wolff/wolff_" + to_string(i++) + ".png");
    }
    // save data
    cout << "Sava data" << endl;
    Array<Array<F64>> data_wo = {wo_time, wo_mag, wo_nrg};
    dat::store_data(data_wo, "data/wolff.dat");

    //              --- plot data
    

    // plot magnitisation
    cout << "plot magnitisation" << endl;
    {

        auto p1 = plt::plot(nm_time, nm_mag, "--gs");
        plt::hold(true);
        auto p2 = plt::plot(am_time, am_mag, "--rs");
        auto p3 = plt::plot(wo_time, wo_mag, "--bs");

        auto l = plt::legend({"Metropolis", "Adaptive Metropolis", "Wolff"});
        l->location(plt::legend::general_alignment::bottomright);
        plt::xlabel("Time in s");
        plt::ylabel("Magnitisation");
        plt::title("T = " + to_string(uint(T)) + ", L = " + to_string(L));
        plt::save("plots/mag.png");
        plt::hold(false);
    }

        // plot magnitisation
    cout << "plot autocorrelation" << endl;
    {
        auto p1 = plt::plot(nm_time, nm_aut, "--gs");
        plt::hold(true);
        auto p2 = plt::plot(am_time, am_aut, "--rs");
        auto p3 = plt::plot(wo_time, wo_aut, "--bs");

        auto l = plt::legend({"Metropolis", "Adaptive Metropolis", "Wolff"});
        l->location(plt::legend::general_alignment::bottomright);
        plt::xlabel("Time in s");
        plt::ylabel("AutoCorrelation");
        plt::title("T = " + to_string(uint(T)) + ", L = " + to_string(L));
        plt::save("plots/aut.png");
        plt::hold(false);
    }

    // plot energy
    cout << wo_nrg.size() << ", " << am_nrg.size();
    cout << "plot energy" << endl;
    {

        auto p1 = plt::loglog(nm_time, nm_nrg, "--gs");
        plt::hold(true);
        auto p2 = plt::loglog(am_time, am_nrg, "--rs");
        auto p3 = plt::loglog(wo_time, wo_nrg, "--bs");

        auto l = plt::legend({"Metropolis SmallStep", 
                              "Adaptive Metropolis", 
                              "Wolff"});
        l->location(plt::legend::general_alignment::bottomright);
        plt::xlabel("Time in s");
        plt::ylabel("Energy in J");
        plt::title("T = " + to_string(uint(T)) + ", L = " + to_string(L));
        plt::save("plots/energy.png");
        plt::hold(false);
    }

    return 0;
}