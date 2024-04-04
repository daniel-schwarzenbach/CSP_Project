#include "Eigen.h++"
#include <eigen3/Eigen/QR>

VectorN fit_lst_sqr(const MatrixNxM& A, const VectorN& b){
    MatrixNxM A_t = A.transpose() * A;
    VectorN b_t = A.transpose() * b;
    return A_t.colPivHouseholderQr().solve(b_t);
}