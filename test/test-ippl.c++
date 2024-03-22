#include <Base.h++>
#include <Timekeeper.h++>
#include <normalVectors.h++>
#include <matplot/matplot.h>
#include <Ippl/Ippl.h>



int main(int mainArgc, char** mainArgs)
{
    //          --- test Ippl
    ippl::Vector<flt, 3> vec3 = {2,3,3};
    cout << vec3[2] << " == 3" << endl << endl;
    ippl::initialize(mainArgc, mainArgs, MPI_COMM_WORLD);

    uint L = 50'000;
    vector<Norm3d>  x3d(L, Norm3d());
    vector<NormPol> xPolar(L, NormPol());
    vector<flt>  buffer(10);
    for(uint i = 0; i < L; ++i){
        x3d[i] = Norm3d();
        xPolar[i] = NormPol();
    }
    
    Timekeeper timekeeper;
    for(uint i = 0; i < L; ++i){
        for(uint j = i; j < L; ++j){
            // cout << i << endl;
            volatile flt x = j;
        }
    }
    timekeeper.stop();
    flt randomtime = timekeeper.time();
    cout << "Time for 50K iterations is: " <<  randomtime << endl;

    timekeeper.start();
    for(uint i = 0; i < L; ++i){
        for(uint j = i; j < L; ++j){
            // cout << i << endl;
            volatile flt x = x3d[i] | x3d[j];
        }
    }
    timekeeper.stop();
    flt timeCar = timekeeper.time();
    cout << "Time for 50K Cartesian calculation is: " 
         << timeCar << endl;

    timekeeper.start();
    for(uint i = 0; i < L; ++i){
        for(uint j = i; j < L; ++j){
            volatile flt x = xPolar[i] | xPolar[j];
        }
    }
    timekeeper.stop();
    flt timePol = timekeeper.time();
    cout << "Time for 50K Polar calculation is: " << timePol
         << endl;

    cout << "timePolar / timeCartesian = " << timePol / timeCar 
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