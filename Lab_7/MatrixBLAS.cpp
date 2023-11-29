#include "MatrixBLAS.h"





MatrixBLAS::MatrixBLAS(){
    this->array = new float[N * N];
    std::memset(this->array, 0, N * N * sizeof(float));
}
MatrixBLAS::~MatrixBLAS(){
    delete this->array;
}
MatrixBLAS::MatrixBLAS(const MatrixBLAS &source){
    MatrixBLAS temp;
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            temp[i][j] = source[i][j];
        }
    }
}
void MatrixBLAS::operator=(const MatrixBLAS &source){
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            (*this)[i][j] = source[i][j];
        }
    }
}
float *MatrixBLAS::operator[](const size_t i){
    return (this->array + (i * N));
}
float *MatrixBLAS::operator[](const size_t i) const {
    return (this->array + (i * N));
}
void MatrixBLAS::operator+=(const MatrixBLAS &source){
    cblas_saxpy(N * N, 1.0, source[0], 1, (*this)[0], 1);
}
void MatrixBLAS::operator-=(const MatrixBLAS &source){
    cblas_saxpy(N * N, -1.0, source[0], 1, (*this)[0], 1);
}
MatrixBLAS MatrixBLAS::operator*(const MatrixBLAS &source){
    MatrixBLAS temp;
    cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans, N, N, N, 1.0, source[0], N, (*this)[0], N, 0.0, temp[0], N);
    return temp;
}
void MatrixBLAS::operator/=(const float divisor){
    cblas_sscal(N * N, 1 / divisor, (*this)[0], 1);
}
float MatrixBLAS::maxSumRows(){
    float maxSum = 0;
    for (size_t i = 0; i < N; ++i){
        float sum = cblas_sasum(N, (*this)[i], 1);
        if (sum > maxSum){
            maxSum = sum;
        }
    }
    return maxSum;
}
float MatrixBLAS::maxSumColumns(){
    float maxSum = 0;
    for (size_t i = 0; i < N; ++i){
        float sum = 0;
        for (size_t j = 0; j < N; ++j){
            sum += (*this)[j][i];
        }
        if (sum > maxSum){
            maxSum = sum;
        }
    }
    return maxSum;
}
void MatrixBLAS::printMatrix(){
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            std::cout << this->array[i * N + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}