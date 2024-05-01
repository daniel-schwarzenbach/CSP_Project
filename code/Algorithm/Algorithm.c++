#include <Algorithm/Algorithm.h++>


algo::AlgoData algo::test_function_delta_t(
        Lattice& lattice,
        flt const &dt, flt const &t_end, flt const& T, flt const& J,
        function<void(Lattice&, flt const&, flt const&, flt const&)> const &algo)
{
    uint maxSize = ceil(t_end/dt);
    Array<flt> time(0);
    time.reserve(maxSize);
    Array<flt> magn(0);
    magn.reserve(maxSize);
    Array<flt> enrg(0);
    enrg.reserve(maxSize);
    Array<flt> auco(0);
    auco.reserve(maxSize);
    measure::LoadingBar lbar(60);
    // not zero
    flt t_elapsed = 0;
    Lattice lcopy = lattice;
    // i = 0;
    auco.push_back(1);
    while (t_elapsed < t_end)
    {
        lbar.update(t_elapsed / t_end * 100.0);
        time.push_back(t_elapsed);
        Vector3 magnVec = measure::get_magnetization(lattice);
        magn.push_back(magnVec.norm());
        enrg.push_back(abs(measure::get_energy(lattice)));

        measure::Timer timer;
        timer.start();
        algo(lattice, dt, T, J);
        timer.stop();
        
        t_elapsed += timer.time();
        flt autcorr = measure::get_auto_correlation(lcopy, lattice);
        auco.push_back(autcorr);
    }

    time.push_back(t_elapsed);
    Vector3 magnVec = measure::get_magnetization(lattice);
    magn.push_back(magnVec.norm());
    enrg.push_back(abs(measure::get_energy(lattice)));
    return {time, magn, enrg, auco};
}