#ifndef __EIGEN_H__
#define __EIGEN_H__

#include <Base.h++>

#include <Eigen/Dense>
// represents an MxN real matrix
using Matrix = Eigen::Matrix<flt, Eigen::Dynamic,
                                Eigen::Dynamic>;

// represents an ℝ³ˣ³ matrix
using Matrix3x3 = Eigen::Matrix<flt, Eigen::Dynamic,
                                Eigen::Dynamic>;
// represents an Nx1 real vector
using Vector = Eigen::Matrix<flt, Eigen::Dynamic, 1>;

// represents an ℝ³ vector
using Vector3 = Eigen::Matrix<flt, 3, 1>;

// returns the solution of the linear system min{ |Ax - b|² }
Vector fit_lst_sqr(const Matrix &A, const Vector &b);

// scalar product of two vectors
flt operator|(const Vector3 &a, const Vector3 &b);

#endif // __EIGEN_H__