#include <iostream>
#include <cstring>
#include <cblas.h>

#define M 10
#define N 2048




class MatrixNaive{
private:
    float* matrix;

public:
    MatrixNaive();
    ~MatrixNaive();
    MatrixNaive(const MatrixNaive &source);
    void operator=(const MatrixNaive &source);
    float * operator[](const size_t i);
    float * operator[](const size_t i) const;
    void operator+=(const MatrixNaive &source);
    void operator-=(const MatrixNaive &source);
    MatrixNaive operator*(const MatrixNaive &source);
    void operator/=(const float divisor);
    float maxSumRows();
    float maxSumColumns();
    void printMatrix();
};