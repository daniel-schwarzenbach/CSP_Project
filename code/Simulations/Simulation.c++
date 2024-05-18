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
    flt t_elapsed = startTime;
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

// cut test_algorithm data to fit to a certain time
Array2D<flt> sim::ns::cut_data_after_time(
        Array2D<flt> const& data, flt const& time)
{
    uint cols = data.size();
    if (cols < 5){
        throw std::runtime_error("data has less than 4 cols");  
    }
    u64 rows = data[0].size();
    for(Array<flt> d : data){
        if (d.size() != rows){
            throw std::runtime_error("data has not the same rows");
        }
    }
    flt t = 0;
    u64 i = 0;
    while (i < rows && t < time){
        t = data[4][i];
        ++i;
    }
    --i;
    Array2D<flt> newData(cols);
    for(uint col = 0; col < cols; ++col){
        newData[col] = sub_range(0,i, data[col]);
    }
    return newData;
}

void sim::ns::fit_datas_after_time(Array2D<flt>& data1,
                                   Array2D<flt>& data2){
    // check for correct sized data!
    uint cols1 = data1.size();
    if (cols1 < 5){
        throw std::runtime_error("data has less than 4 cols");  
    }
    u64 rows1 = data1[0].size();
    for(Array<flt> d : data1){
        if (d.size() != rows1){
            throw std::runtime_error("data has not the same rows");
        }
    }
    uint cols2 = data2.size();
    if (cols2 < 5){
        throw std::runtime_error("data has less than 4 cols");  
    }
    u64 rows2 = data2[0].size();
    for(Array<flt> d : data1){
        if (d.size() != rows2){
            throw std::runtime_error("data has not the same rows");
        }
    }
    // check witch data has the shorter time span
    flt t1 = data1[4][rows1-1];
    flt t2 = data2[4][rows2-1];
    if(t1 < t2){
        data2 = sim::ns::cut_data_after_time(data2, t1);
    } 
    else if (t2 < t1){
        data1 = sim::ns::cut_data_after_time(data1, t2);
    }
}