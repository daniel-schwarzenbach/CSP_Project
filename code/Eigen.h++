#ifndef __EIGEN_H__
#define __EIGEN_H__

#include <Base.h++>

#include <eigen3/Eigen/Dense>
// represents an NxN real matrix
using MatrixNxM = Eigen::Matrix<flt, Eigen::Dynamic, 
                                Eigen::Dynamic, Eigen::RowMajor>;
// represents an Nx1 real vector
using VectorN = Eigen::Matrix<flt, Eigen::Dynamic, 1>;

// represents an ℝ³ vector
using Vector3 = Eigen::Matrix<flt, 3, 1>;

// returns the solution of the linear system min{ |Ax - b|² }
VectorN fit_lst_sqr(const MatrixNxM& A, const VectorN& b);



#endif // __EIGEN_H__