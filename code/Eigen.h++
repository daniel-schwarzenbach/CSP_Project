#ifndef __EIGEN_H__
#define __EIGEN_H__
/*
includes the eigen files and defines our vectors and matices
*/

#include <Base.h++>
#include <Eigen/Dense>
// represents an ℝ^3x3 matrix
using Matrix = Eigen::Matrix<f32, Eigen::Dynamic,
                             Eigen::Dynamic>;

// represents an ℝ^3x3 matrix
using Matrix3x3 = Eigen::Matrix<f32, 3, 3>;

// represents an ℝ^N vector
using Vector = Eigen::Matrix<f32, Eigen::Dynamic, 1>;

// represents an ℝ^3 vector
using Vector3 = Eigen::Matrix<f32, 3, 1>;


/*
solves the least square problem:  argmin(x){ |Ax - b|² }

/ @param A: an n x m Matrix
/ @param b: an Vector of lenth n
/ @return x: an Vector of lenth m
*/
Vector fit_lst_sqr(const Matrix &A, const Vector &b);

// scalar product of two vectors, a ⋅ b
f32 operator|(const Vector3 &a, const Vector3 &b);

#endif // __EIGEN_H__