#include "MatrixNaive.h"





MatrixNaive::MatrixNaive(){
    matrix = new float[N * N];
    std::memset(matrix, 0, N * N * sizeof(float));
}
MatrixNaive::~MatrixNaive(){
    delete matrix;
}
MatrixNaive::MatrixNaive(const MatrixNaive &source){
    MatrixNaive temp;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            temp[i][j] = source[i][j];
        }
    }
}
void MatrixNaive::operator=(const MatrixNaive &source){
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            (*this)[i][j] = source[i][j];
        }
    }
}
float *MatrixNaive::operator[](const size_t i){
    return (matrix + (i * N));
}
float *MatrixNaive::operator[](const size_t i) const{
    return (matrix + (i * N));
}
void MatrixNaive::operator+=(const MatrixNaive &source){
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            (*this)[i][j] = (*this)[i][j] + source[i][j];
        }
    }
}
void MatrixNaive::operator-=(const MatrixNaive &source){
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            (*this)[i][j] -= source[i][j];
        }
    }
}
MatrixNaive MatrixNaive::operator*(const MatrixNaive &source){
    MatrixNaive temp;
    for (size_t i = 0; i < N; i++) {
        for (size_t k = 0; k < N; k++) {
            for (size_t j = 0; j < N; j++) {
                temp[i][j] += (*this)[i][k] * source[k][j];
            }
        }
    }
    return temp;
}
void MatrixNaive::operator/=(const float divisor){
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            (*this)[i][j] /= (float)divisor;
        }
    }
}
float MatrixNaive::maxSumRows(){
    float maxSum = 0;
    for (size_t i = 0; i < N; i++) {
        float sum = 0;
        for (size_t j = 0; j < N; j++) {
            sum += (*this)[i][j];
        }
        if (sum > maxSum) {
            maxSum = sum;
        }
    }
    return maxSum;
}
float MatrixNaive::maxSumColumns(){
    float maxSum = 0;
    for (size_t i = 0; i < N; i++) {
        float sum = 0;
        for (size_t j = 0; j < N; j++) {
            sum += (*this)[j][i];
        }
        if (sum > maxSum) {
            maxSum = sum;
        }
    }
    return maxSum;
}
void MatrixNaive::printMatrix(){
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            std::cout << matrix[i * N + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}