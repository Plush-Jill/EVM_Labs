#include <iostream>
#include <cstring>
#include <immintrin.h>
#include <ia32intrin.h>
#include <mmintrin.h>

#define M 10
#define N 2048



class MatrixVectorize {
private:
    float *array;

public:
    MatrixVectorize();
    ~MatrixVectorize();
    MatrixVectorize(const MatrixVectorize &source);
    void operator=(const MatrixVectorize &source);
    float * operator[](const size_t i);
    float * operator[](const size_t i) const;
    void operator+=(const MatrixVectorize &source);
    MatrixVectorize operator-(const MatrixVectorize &source);
    void operator-=(const MatrixVectorize &source);
    MatrixVectorize operator*(const MatrixVectorize &source);
    void operator/=(const float divisor);
    float maxSumRows();
    float maxSumColumns();
    void printMatrix();
};