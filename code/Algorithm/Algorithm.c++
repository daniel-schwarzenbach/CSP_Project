#include <Algorithm/Algorithm.h++>

Array2D<flt> algo::dt::test_algorithm(
    Lattice &lattice,
    flt const &dt, flt const &t_end, flt const &T, flt const &J,
    algo::dt::Algorithm const &algo)
{
    Vector3 z = {0,0,1};
    uint maxSize = ceil(flt(t_end) / flt(dt));
    Array<flt> Time(0);
    Time.reserve(maxSize);
    Array<flt> M(0);
    M.reserve(maxSize);
    Array<flt> E(0);
    E.reserve(maxSize);
    Array<flt> M_z(0);
    M_z.reserve(maxSize);
    measure::LoadingBar lbar(60);
    // not zero
    flt t_elapsed = 0;
    while (t_elapsed < t_end)
    {
        lbar.update(t_elapsed / t_end * 100.0);
        Vector3 magnVec = measure::get_magnetization(lattice);
        Time.push_back(t_elapsed);
        M.push_back(magnVec.norm());
        M_z.push_back(magnVec | z);
        E.push_back(measure::get_energy(lattice));
        measure::Timer watch; watch.start();
        algo(lattice, dt, T, J);
        watch.stop();
        t_elapsed += watch.time();
    }

    Time.push_back(t_elapsed);
    Vector3 magnVec = measure::get_magnetization(lattice);
    M.push_back(magnVec.norm());
    M_z.push_back(magnVec | z);
    E.push_back(abs(measure::get_energy(lattice)));
    return {Time, M, M_z, E};
}

Array2D<flt> algo::ds::test_algorithm(
    Lattice &lattice,
    uint const &ds, uint const &numSteps, flt const &T, flt const &J,
    algo::ds::Algorithm const &algo)
{
    Vector3 z = {0,0,1};
    uint maxSize = ceil(flt(numSteps) / flt(ds));
    Array<flt> Time(0);
    Time.reserve(0);
    Array<flt> Step(0);
    Step.reserve(maxSize);
    Array<flt> M(0);
    M.reserve(maxSize);
    Array<flt> E(0);
    E.reserve(maxSize);
    Array<flt> M_z(0);
    M_z.reserve(maxSize);
    measure::LoadingBar lbar(60);
    // not zero
    flt step = 0;
    flt t_elapsed = 0;
    while (step < numSteps)
    {
        lbar.update(step / numSteps * 100.0);
        
        Vector3 magnVec = measure::get_magnetization(lattice);
        Step.push_back(step);
        M.push_back(magnVec.norm());
        M_z.push_back(magnVec | z);
        E.push_back(measure::get_energy(lattice));
        measure::Timer timer; timer.start();
        algo(lattice, ds, T, J);
        timer.stop();
        t_elapsed += timer.time();
        step += ds;
    }

    Step.push_back(step);
    Vector3 magnVec = measure::get_magnetization(lattice);
    M.push_back(magnVec.norm());
    M_z.push_back(magnVec | z);
    E.push_back(measure::get_energy(lattice));
    return {Step, M, M_z, E};
}