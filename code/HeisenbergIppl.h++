#ifndef __HEISENBERGIPPL_H__
#define __HEISENBERGIPPL_H__

#include <Ippl.h>

constexpr uint Dim = 3;

using Mesh      = ippl::UniformCartesian<double, 3>;
using Centering = Mesh::DefaultCentering;
using Lattice   = ippl::Field<double, Dim, Mesh, Centering>;


#endif // __HEISENBERGIPPL_H__