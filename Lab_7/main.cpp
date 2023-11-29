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
    MatrixNaive ma, mb, mr, mo;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            float a = rand() % 100;
            ma[i][j] = a;
            mb[j][i] = a;
            mr[i][j] = 0;
            mo[i][j] = 0;
        }
        mr[i][i] = 1;
        mo[i][i] = 1;
    }

    mb /= (ma.maxSumRows() * ma.maxSumColumns());
    mr -= (mb * ma);

    MatrixNaive mrc;
    mrc = mr;
    for (size_t i = 1; i < M + 1; i++) {
        mo += mr;
        mr = mr * mrc;
    }
    mo = mo * mb;

    std::cout << "naive" << std::endl;
    ma.printMatrix();
    mo.printMatrix();
}
void useVectorzie(){
    srand(100);
    MatrixVectorize ma, mb, mr, mo;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            float a = rand() % 100;
            ma[i][j] = a;
            mb[j][i] = a;
            mr[i][j] = 0;
            mo[i][j] = 0;
        }
        mr[i][i] = 1;
        mo[i][i] = 1;
    }

    mb /= (ma.maxSumRows() * ma.maxSumColumns());
    mr -= (mb * ma);

    MatrixVectorize mrc;
    mrc = mr;
    for (size_t i = 1; i < M + 1; i++) {
        mo += mr;
        mr = mr * mrc;
    }
    mo = mo * mb;

    std::cout << "Vect" << std::endl;
    ma.printMatrix();
    mo.printMatrix();

}
void useBLAS(){
    srand(100);
    MatrixBLAS ma, mb, mr, mo;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            float a = rand() % 100;
            ma[i][j] = a;
            mb[j][i] = a;
            mr[i][j] = 0;
            mo[i][j] = 0;
        }
        mr[i][i] = 1;
        mo[i][i] = 1;
    }

    mb /= (ma.maxSumRows() * ma.maxSumColumns());
    mr -= (mb * ma);

    MatrixBLAS mrc;
    mrc = mr;
    for (size_t i = 1; i < M + 1; i++) {
        mo += mr;
        mr = mr * mrc;
    }
    mo = mo * mb;

    std::cout << "blas2" << std::endl;
    ma.printMatrix();
    mo.printMatrix();

}


int main(int argc, char** argv){
    if (argc > 1){
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
    }
    /*srand(100);
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
    D.printMatrix();*/

    return 0;
}