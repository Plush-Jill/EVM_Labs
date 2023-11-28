#include <iostream>
#include <cstring>
#include <cblas.h>

#define M 10
#define N 2048




class MatrixBLAS{
private:
    float* array;

public:
    MatrixBLAS();
    ~MatrixBLAS();
    MatrixBLAS(const MatrixBLAS &source);

    void operator=(const MatrixBLAS &source);
    float* operator[](const size_t i);
    float* operator[](const size_t i) const;
    void operator+=(const MatrixBLAS &source);
    void operator-=(const MatrixBLAS &source);
    MatrixBLAS operator*(const MatrixBLAS &source);
    void operator/=(const float divisor);
    float maxSumRows();
    float maxSumColumns();
    void printMatrix();
};