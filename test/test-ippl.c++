#include <Base.h++>
#include <Ippl.h>

int main()
{
    ippl::Vector<flt, 3> vec3 = {2,3,3};
    cout << vec3[2] << " == 3" << endl << endl;
}