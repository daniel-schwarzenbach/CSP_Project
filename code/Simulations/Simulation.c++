#include <Simulations/Simulation.h++>

// simulation for dt in Time
Array2D<flt> sim::dt::test_algorithm(
    Lattice3D<Spin> &lattice,
    flt const &dt, flt const &t_end, flt const &T, flt const &J,
    sim::dt::Algorithm const &algo, bool loadingBar)
{
    // get z Vector for measuring Mz
    Vector3 z = {0, 0, 1};
    // get max number of runs for reserving vectors
    uint maxSize = ceil(flt(t_end) / flt(dt));

    // init data arrays, reserve to avoid time to reallocate data
    Array<flt> Time(0);
    Time.reserve(maxSize);
    Array<flt> M(0);
    M.reserve(maxSize);
    Array<flt> E(0);
    E.reserve(maxSize);
    Array<flt> M_z(0);
    M_z.reserve(maxSize);

    // init loding bar
    measure::LoadingBar lbar(60);
    // measure elapsed time
    flt t_elapsed = 0;
    while (t_elapsed < t_end)
    {
        // update loading bar if requested
        if (loadingBar)
            lbar.update(t_elapsed / t_end * 100.0);
        // calculate data
        Vector3 magnVec = measure::get_magnetization(lattice);
        Time.push_back(t_elapsed);
        M.push_back(magnVec.norm());
        M_z.push_back(magnVec | z);
        E.push_back(measure::get_energy(lattice));
        // track performance
        measure::Timer watch;
        // run algoritm
        algo(lattice, dt, T, J);
        watch.stop();
        // update time
        t_elapsed += watch.time();
    }
    // update loading bar if requested
    if (loadingBar)
        lbar.update(100.0);
    // update data
    Time.push_back(t_elapsed);
    Vector3 magnVec = measure::get_magnetization(lattice);
    M.push_back(magnVec.norm());
    M_z.push_back(magnVec | z);
    E.push_back(abs(measure::get_energy(lattice)));
    // return data
    return {Time, M, M_z, E};
}

Array2D<flt> sim::ns::test_algorithm(
        Lattice3D<Spin> &lattice,
        uint const &ns, u64 const &numSteps, flt const &T,
        flt const &J,
        Spin const &h, Spin const &k,
        Algorithm const &algorithmus,
        bool const &loadingBar,
        u64 const& startStep,
        flt const& startTime)
{
    // get z Vector for measuring Mz
    Vector3 z = {0, 0, 1};
    // get max number of runs for reserving vectors
    uint maxSize = ceil(flt(numSteps) / flt(ns));

    // init data arrays, reserve to avoid time to reallocate data
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

    // init loding bar
    measure::LoadingBar lbar(60);
    // measure steps and elapsed time
    flt t_elapsed = 0;
    u64 step = startStep;
    const u64 maxSteps = numSteps + startStep;
    while (step < maxSteps)
    {
        // update loading bar if requested
        if (loadingBar)
            lbar.update(flt(step) / flt(numSteps) * 100.0);
        // calculate data
        Vector3 magnVec = measure::get_magnetization(lattice);
        Step.push_back(step);
        M.push_back(magnVec.norm());
        M_z.push_back(magnVec | z);
        E.push_back(measure::get_energy(lattice));
        Time.push_back(t_elapsed);
        // track performance
        measure::Timer timer;
        timer.start();
        // run algorithm
        algorithmus(lattice, ns, T, J, h, k);
        timer.stop();
        // update time
        t_elapsed += timer.time();
        // update number of steps
        step += ns;
    }
    if (loadingBar)
        lbar.update(100);
    Step.push_back(step);
    Vector3 magnVec = measure::get_magnetization(lattice);
    M.push_back(magnVec.norm());
    M_z.push_back(magnVec | z);
    E.push_back(measure::get_energy(lattice));
    Time.push_back(t_elapsed);
    return {Step, M, M_z, E, Time};
}