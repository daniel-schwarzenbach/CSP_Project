#include <Base.h++>
#include <Timekeeper.h++>
#include <Spin.h++>
#include <matplot/matplot.h>


int main(int mainArgCount, char** mainArgs)
{

    uint L = 150'000;
    vector<Spin3d>  x3d(L, Spin3d());
    vector<SpinPol> xPolar(L, SpinPol());
    vector<SpinFast> xFast(L, SpinFast());
    vector<flt>  buffer(10);
    for(uint i = 0; i < L; ++i){
        x3d[i] = Spin3d();
        xPolar[i] = SpinPol();
        xFast[i] = SpinFast();
    }
    
    Timekeeper timekeeper;
    for(uint i = 0; i < L; ++i){
        for(uint j = i; j < L; ++j){
            // cout << i << endl;
            volatile flt x = j;
        }
    }
    timekeeper.stop();
    flt iterationTime = timekeeper.time();
    cout << "Time for "<< L/1'000 << "K iterations is: " 
         <<  iterationTime << endl;

    timekeeper.start();
    for(uint i = 0; i < L; ++i){
        for(uint j = i; j < L; ++j){
            // cout << i << endl;
            volatile flt x = x3d[i] | x3d[j];
        }
    }
    timekeeper.stop();
    flt timeCar = timekeeper.time();
    cout << "Time for "<< L/1'000 << "K Cartesian calculation is: " 
         << timeCar << endl;

    timekeeper.start();
    for(uint i = 0; i < L; ++i){
        for(uint j = i; j < L; ++j){
            // cout << i << endl;
            volatile flt x = xFast[i] | xFast[j];
        }
    }
    timekeeper.stop();
    flt timeFast = timekeeper.time();
    cout << "Time for "<< L/1'000 << "K CartesianFast calculation is: " 
         << timeFast << endl;

    timekeeper.start();
    for(uint i = 0; i < L; ++i){
        for(uint j = i; j < L; ++j){
            volatile flt x = xPolar[i] | xPolar[j];
        }
    }
    timekeeper.stop();
    flt timePol = timekeeper.time();
    cout << "Time for "<< L/1'000 << "K Polar calculation is: " 
         << timePol
         << endl;

    cout << "timePolar / timeCartesian = " 
         << (timePol-iterationTime) 
            / (std::min(timeCar,timeFast)-iterationTime)
         << endl;
    //          --- test Matplot++
    namespace plt = matplot; 
    vector<double> x = plt::linspace(0, ₂π);
    vector<double> y = plt::transform(x, [](auto x){return sin(x);});
    // plot sin
    plt::plot(x, y, "-o");
    // enables plot changing
    plt::hold(true);
    // plot -sin
    plt::plot(x, plt::transform(y, [](auto y) { return -y; }),"--xr");
    plt::plot(x, plt::transform(x, [](auto x) { return x / π - 1.; }),
              "-:gs");
    // plot some data points
    plt::plot({1.0, 0.7, 0.4, 0.0, -0.4, -0.7, -1}, "k");

    plt::save("testplot.png");
}