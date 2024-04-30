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
const flt J = 1.0;
// maximal runtime
const flt dt = 0.01;
// end Time
const flt t_end = 5.0;
// Temerature
const flt T = 0.5;
// int Random Lattice Seed
const int seed = 69; // hi hi

// side Lenth
const uint L = 10;

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
    // metropolis small step
    Array<flt> ms_time(0);
    ms_time.reserve(size);
    Array<flt> ms_mag(0);
    ms_mag.reserve(size);
    Array<flt> ms_nrg(0);
    ms_nrg.reserve(size);
    Array<flt> ms_aut(0);
    ms_aut.reserve(size);
    // metropolis adaptive
    Array<flt> am_time(0);
    am_time.reserve(size);
    Array<flt> am_mag(0);
    am_mag.reserve(size);
    Array<flt> am_nrg(0);
    am_nrg.reserve(size);
    Array<flt> am_aut(0);
    am_aut.reserve(size);
    // metropolis random
    Array<flt> mr_time(0);
    mr_time.reserve(size);
    Array<flt> mr_mag(0);
    mr_mag.reserve(size);
    Array<flt> mr_nrg(0);
    mr_nrg.reserve(size);
    Array<flt> mr_aut(0);
    mr_aut.reserve(size);

    Array<flt> wo_time(0);
    wo_time.reserve(size);
    Array<flt> wo_mag(0);
    wo_mag.reserve(size);
    Array<flt> wo_nrg(0);
    wo_nrg.reserve(size);
    Array<flt> wo_aut(0);
    wo_aut.reserve(size);

    //             --- metropolis small step
    cout << "running metropolis small step ..." << endl;
    {
        LoadingBar lbar(60);
        // not zero
        flt t_elapsed = dt;
        int i = 0;
        ms_aut.push_back(1);
        while (t_elapsed < t_end)
        {
            lbar.update(t_elapsed / t_end * 100.0);
            // dat::plot_lattice(
            //         lattice,
            //         "plots/addMetro/metro_" + to_string(i++) + ".png");

            ms_time.push_back(t_elapsed);
            Vector3 magn = measure::get_magnetization(lattice);
            ms_mag.push_back(magn.norm());
            ms_nrg.push_back(abs(measure::get_energy(lattice)));
            TimeKeeper timer;
            timer.start();
            metropolis(lattice, T, J, dt, maxUint, MoveType::SmallStep);
            timer.stop();
            t_elapsed += timer.time();
            flt aut = measure::get_auto_correlation(lcopy, lattice);
            ms_aut.push_back(aut);
        }

        ms_time.push_back(t_elapsed);
        Vector3 magn = measure::get_magnetization(lattice);
        ms_mag.push_back(magn.norm());
        ms_nrg.push_back(abs(measure::get_energy(lattice)));
        // dat::plot_lattice(
        //             lattice,
        //             "plots/addMetro/metro_" + to_string(i++) + ".png");
    }
    dat::plot_lattice(lattice, "bMet.png");

    // save data
    cout << "Save metropolis small step data" << endl;
    Array<Array<flt>> data_nm = {ms_time, ms_mag, ms_nrg};
    dat::store_data(data_nm, "data/metr_small_" + to_str(T) + ".dat");

    rng::set_seed(seed);
    lattice.randomize();

    //             --- metropolis random
    cout << "running metropolis random step ..." << endl;
    {
        LoadingBar lbar(60);
        // not zero
        flt t_elapsed = dt;
        int i = 0;
        mr_aut.push_back(1);
        while (t_elapsed < t_end)
        {
            lbar.update(t_elapsed / t_end * 100.0);
            // dat::plot_lattice(
            //         lattice,
            //         "plots/addMetro/metro_" + to_string(i++) + ".png");

            mr_time.push_back(t_elapsed);
            Vector3 magn = measure::get_magnetization(lattice);
            mr_mag.push_back(magn.norm());
            mr_nrg.push_back(abs(measure::get_energy(lattice)));
            TimeKeeper timer;
            timer.start();
            metropolis(lattice, T, J, dt, maxUint, MoveType::Random);
            timer.stop();
            t_elapsed += timer.time();
            flt aut = measure::get_auto_correlation(lcopy, lattice);
            mr_aut.push_back(aut);
        }

        mr_time.push_back(t_elapsed);
        Vector3 magn = measure::get_magnetization(lattice);
        mr_mag.push_back(magn.norm());
        mr_nrg.push_back(abs(measure::get_energy(lattice)));
        // dat::plot_lattice(
        //             lattice,
        //             "plots/addMetro/metro_" + to_string(i++) + ".png");
    }
    dat::plot_lattice(lattice, "bMet.png");

    // save data
    cout << "Save metropolis small step data" << endl;
    Array<Array<flt>> data_mr = {mr_time, mr_mag, mr_nrg, mr_aut};
    dat::store_data(data_mr, "data/metr_small_" + to_str(T) + ".dat");

    rng::set_seed(seed);
    lattice.randomize();

    //             --- adaptive-metropolis
    cout << "running addaptive metropolis ..." << endl;
    {
        am_aut.push_back(1);
        LoadingBar lbar(60);
        // not zero
        flt t_elapsed = dt;
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

            flt aut = measure::get_auto_correlation(lcopy, lattice);
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
    Array<Array<flt>> data_am = {am_time, am_mag, am_nrg};
    dat::store_data(data_am, "data/addmetr_" + to_str(T) + ".dat");

    //              --- randomiz Lattice
    rng::set_seed(seed);
    lattice.randomize();

    //             --- wolff
    cout << "running wolff ..." << endl;
    {
        wo_aut.push_back(1);
        LoadingBar lbar(60);
        flt t_elapsed = dt;
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

            flt aut = measure::get_auto_correlation(lcopy, lattice);
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
    Array<Array<flt>> data_wo = {wo_time, wo_mag, wo_nrg};
    dat::store_data(data_wo, "data/wolff_" + to_str(T) + ".dat");

    //              --- plot data

    // plot magnitisation
    cout << "plot magnitisation" << endl;
    {

        auto p1 = plt::plot(mr_time, mr_mag, "--gs");
        plt::hold(true);
        auto p4 = plt::plot(ms_time, ms_mag, "--ys");
        auto p2 = plt::plot(am_time, am_mag, "--rs");
        auto p3 = plt::plot(wo_time, wo_mag, "--bs");

        auto l = plt::legend({"Metropolis Random",
                              "Metropolis Small Step", "Adaptive Metropolis",
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
        auto p1 = plt::plot(mr_time, mr_aut, "--gs");
        plt::hold(true);
        auto p4 = plt::plot(ms_time, ms_aut, "--ys");
        auto p2 = plt::plot(am_time, am_aut, "--rs");
        auto p3 = plt::plot(wo_time, wo_aut, "--bs");

        auto l = plt::legend({"Metropolis Random",
                              "Metropolis Small Step", "Adaptive Metropolis",
                              "Wolff"});
        l->location(plt::legend::general_alignment::topright);
        plt::xlabel("Time in s");
        plt::ylabel("AutoCorrelation");
        plt::title("T = " + to_str(T) + ", L = " + to_string(L));
        plt::save("plots/aut_" + to_str(T) + ".png");
        plt::hold(false);
    }

    // plot energy
    cout << wo_nrg.size() << ", " << am_nrg.size();
    cout << "plot energy" << endl;
    {
        auto p1 = plt::plot(mr_time, mr_nrg, "--gs");
        plt::hold(true);
        auto p4 = plt::plot(ms_time, ms_nrg, "--ys");
        auto p2 = plt::plot(am_time, am_nrg, "--rs");
        auto p3 = plt::plot(wo_time, wo_nrg, "--bs");

        auto l = plt::legend({"Metropolis Random",
                              "Metropolis Small Step", "Adaptive Metropolis",
                              "Wolff"});
        l->location(plt::legend::general_alignment::bottomright);
        plt::xlabel("Time in s");
        plt::ylabel("Energy in J");
        plt::title("T = " + to_str(T) + ", L = " + to_string(L));
        plt::save("plots/energy_" + to_str(T) + ".png");
        plt::hold(false);
    }
    return 0;
}