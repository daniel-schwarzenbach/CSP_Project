#include <Base.h++>
#include <Ippl/Ippl.h>


int main(int mainArgc, char** mainArgs)
{
    ippl::Vector<flt, 3> vec3 = {2,3,3};
    cout << vec3[2] << " == 3" << endl << endl;

    ippl::initialize(mainArgc, mainArgs, MPI_COMM_WORLD);
}