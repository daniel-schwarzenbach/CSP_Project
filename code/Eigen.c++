#include "Eigen.h++"
#include <Eigen/QR>

Vector fit_lst_sqr(const Matrix &A, const Vector &b)
{
    // solves the LSE: AᵀAx = Aᵀb
    Matrix A_t = A.transpose() * A;
    Vector b_t = A.transpose() * b;
    return A_t.colPivHouseholderQr().solve(b_t);
}

// scalar product
f32 operator|(const Vector3 &a, const Vector3 &b)
{
    return a.dot(b);
}