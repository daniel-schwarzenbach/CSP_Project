#include <Base.h++>
#include <Timekeeper.h++>
#include <Spin.h++>
#include <matplot/matplot.h>


int main(int mainArgCount, char** mainArgs)
{

    uint L = 50'000;
    vector<Spin3d>  x3d(L, Spin3d());
    vector<SpinPol> xPolar(L, SpinPol());
    //vector<SpinFast> xFast(L, SpinFast());
    vector<flt>  buffer(10);
    for(uint i = 0; i < L; ++i){
        x3d[i] = Spin3d();
        xPolar[i] = SpinPol();
        //xFast[i] = SpinFast();
    }
    
    TimeKeeper timekeeper;
    for(uint i = 0; i < L; ++i){
        for(uint j = 0; j < L; ++j){
            // cout << i << endl;
            volatile flt x = j;
        }
    }
    timekeeper.stop();
    double iterationTime = timekeeper.time();
    cout << "Time for "<< L/1'000 << "K iterations is: " 
         <<  iterationTime << endl;

    timekeeper.start();
    for(uint i = 0; i < L; ++i){
        for(uint j = 0; j < L; ++j){
            // cout << i << endl;
            volatile flt x = x3d[i] | x3d[j];
        }
    }
    timekeeper.stop();
    double timeCar = timekeeper.time();
    cout << "Time for "<< L/1'000 << "K Cartesian calculation is: " 
         << timeCar << endl;

    // timekeeper.start();
    // for(uint i = 0; i < L; ++i){
    //     for(uint j = i; j < L; ++j){
    //         // cout << i << endl;
    //         volatile flt x = xFast[i] | xFast[j];
    //     }
    // }
    // timekeeper.stop();
    // flt timeFast = timekeeper.time();
    // cout << "Time for "<< L/1'000 << "K CartesianFast calculation is: " 
    //      << timeFast << endl;

    timekeeper.start();
    for(uint i = 0; i < L; ++i){
        for(uint j = 0; j < L; ++j){
            volatile flt x = xPolar[i] | xPolar[j];
        }
    }
    timekeeper.stop();
    double timePol = timekeeper.time();
    cout << "Time for "<< L/1'000 << "K Polar calculation is: " 
         << timePol
         << endl;

    double performPol = (timePol-iterationTime) * (1e+9 / (L*L));
    double performCar = (timeCar-iterationTime) * (1e+9 / (L*L));

    cout << endl << "spinPol time :"<< performPol <<" ns" << endl;
    cout << endl << "spinCar time :" << performCar<<" ns" << endl;


    cout << "timePolar / timeCartesian = " 
         << (timePol-iterationTime) 
            / (timeCar-iterationTime)
         << endl;
    
}