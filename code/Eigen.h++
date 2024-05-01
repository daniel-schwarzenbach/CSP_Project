#ifndef __EIGEN_H__
#define __EIGEN_H__
/*
includes the eigen files and defines our vectors and matices
*/

#include <Base.h++>
#include <Eigen/Dense>
// represents an MxN real matrix
using Matrix = Eigen::Matrix<f32, Eigen::Dynamic,
                             Eigen::Dynamic>;

// represents an ℝ³ˣ³ matrix
using Matrix3x3 = Eigen::Matrix<f32, Eigen::Dynamic,
                                Eigen::Dynamic>;
// represents an Nx1 real vector
using Vector = Eigen::Matrix<f32, Eigen::Dynamic, 1>;

// represents an ℝ³ vector
using Vector3 = Eigen::Matrix<f32, 3, 1>;

// returns the solution of the linear system min{ |Ax - b|² }
Vector fit_lst_sqr(const Matrix &A, const Vector &b);

// scalar product of two vectors
f32 operator|(const Vector3 &a, const Vector3 &b);

#endif // __EIGEN_H__