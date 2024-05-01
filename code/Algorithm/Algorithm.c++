#include <Algorithm/Algorithm.h++>

Array2D<flt> algo::dt::test_algorithm(
    Lattice &lattice,
    flt const &dt, flt const &t_end, flt const &T, flt const &J,
    algo::dt::Algorithm const &algo)
{
    uint maxSize = ceil(t_end / dt);
    Array<flt> time(0);
    time.reserve(maxSize);
    Array<flt> magn(0);
    magn.reserve(maxSize);
    Array<flt> enrg(0);
    enrg.reserve(maxSize);
    measure::LoadingBar lbar(60);
    // not zero
    flt t_elapsed = 0;
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
    }

    time.push_back(t_elapsed);
    Vector3 magnVec = measure::get_magnetization(lattice);
    magn.push_back(magnVec.norm());
    enrg.push_back(abs(measure::get_energy(lattice)));
    return {time, magn, enrg};
}