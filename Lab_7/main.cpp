#include <iostream>
#include <immintrin.h>
#include "MatrixBLAS.h"


#define M 10
#define N 2048



int main() {
    
    srand(100);
    MatrixBLAS A, B, C, D;
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            float a = rand() % 100;
            A[i][j] = a;
            B[j][i] = a;
            C[i][j] = 0;
            D[i][j] = 0;
        }
        C[i][i] = 1;
        D[i][i] = 1;
    }

    B /= (A.maxSumRows() * A.maxSumColumns());
    C -= (B * A);

    MatrixBLAS mrc;
    mrc = C;
    for (size_t i = 1; i < M + 1; ++i){
        D += C;
        C = C * mrc;
    }
    D = D * B;

    std::cout << "blas2" << std::endl;
    A.printMatrix();
    D.printMatrix();

    return 0;
}