#include <iostream>
#include <immintrin.h>
#include "MatrixNaive.h"
#include "MatrixVectorize.h"
#include "MatrixBLAS.h"
#include <string>

#define M 10
#define N 2048

void useNaive(){
    srand(100);
    MatrixNaive A;
    MatrixNaive B;
    MatrixNaive R;
    MatrixNaive inverseMatrix;
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            float a = rand() % 100;
            A[i][j] = a;
            B[j][i] = a;
            R[i][j] = 0;
            inverseMatrix[i][j] = 0;
        }
        R[i][i] = 1;
        inverseMatrix[i][i] = 1;
    }

    B /= (A.maxSumRows() * A.maxSumColumns());
    R -= (B * A);

    MatrixNaive matrixF;
    matrixF = R;
    for (size_t i = 1; i < M + 1; ++i){
        inverseMatrix += R;
        R = R * matrixF;
    }
    inverseMatrix = inverseMatrix * B;

    std::cout << "naive" << std::endl;
    A.printMatrix();
    inverseMatrix.printMatrix();
}
void useVectorzie(){
    srand(100);
    MatrixVectorize A;
    MatrixVectorize B;
    MatrixVectorize R;
    MatrixVectorize inverseMatrix;
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            float a = rand() % 100;
            A[i][j] = a;
            B[j][i] = a;
            R[i][j] = 0;
            inverseMatrix[i][j] = 0;
        }
        R[i][i] = 1;
        inverseMatrix[i][i] = 1;
    }

    B /= (A.maxSumRows() * A.maxSumColumns());
    R -= (B * A);

    MatrixVectorize matrixF;
    matrixF = R;
    for (size_t i = 1; i < M + 1; ++i){
        inverseMatrix += R;
        R = R * matrixF;
    }
    inverseMatrix = inverseMatrix * B;

    std::cout << "Vect" << std::endl;
    A.printMatrix();
    inverseMatrix.printMatrix();

}
void useBLAS(){
    srand(100);
    MatrixBLAS A;
    MatrixBLAS B;
    MatrixBLAS R;
    MatrixBLAS inverseMatrix;
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            float a = rand() % 100;
            A[i][j] = a;
            B[j][i] = a;
            R[i][j] = 0;
            inverseMatrix[i][j] = 0;
        }
        R[i][i] = 1;
        inverseMatrix[i][i] = 1;
    }

    B /= (A.maxSumRows() * A.maxSumColumns());
    R -= (B * A);

    MatrixBLAS matrixF;
    matrixF = R;
    for (size_t i = 1; i < M + 1; ++i){
        inverseMatrix += R;
        R = R * matrixF;
    }
    inverseMatrix = inverseMatrix * B;

    std::cout << "blas2" << std::endl;
    A.printMatrix();
    inverseMatrix.printMatrix();

}


int main(int argc, char** argv){


    /*if (argc > 1){
        if (std::string(argv[1]) == "-naive"){
            useNaive();
        }else if (std::string(argv[1]) == "-BLAS"){
            useBLAS();
        }else if (std::string(argv[1]) == "-vectorize"){
            useVectorzie();
        }else{
            return 1;
        }
    }else{
        std::cerr << "No arguments." << std::endl;
    }*/



    return 0;
}