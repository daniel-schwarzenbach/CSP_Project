#include <Base.h++>
#include <Kokkos_Macros.hpp>
#include <Kokkos_Profiling_ProfileSection.hpp>
#include <Kokkos_Profiling_ScopedRegion.hpp>
#include <Ippl.h>


int main(int mainArgc, char** mainArgs)
{
    ippl::Vector<flt, 3> vec3 = {2,3,3};
    cout << vec3[2] << " == 3" << endl << endl;

    //ippl::initialize(mainArgc, mainArgs, MPI_COMM_WORLD);
}