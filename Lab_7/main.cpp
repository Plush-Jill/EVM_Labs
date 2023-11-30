#include <iostream>
#include <cstring>
#include <cblas.h>
#include <immintrin.h>


#define M 10
#define N 2048


class MatrixNaive{
private:

public:
    float* array{};
    MatrixNaive(){
        array = new float[N * N];
        std::memset(array, 0, N * N * sizeof(float));
    }
    ~MatrixNaive(){
        delete array;
    }
    MatrixNaive(const MatrixNaive &source){
        for (size_t i = 0; i < N; ++i){
            for (size_t j = 0; j < N; ++j){
                *this[i][j] = source[i][j];
            }
        }
    }


    MatrixNaive& operator=(const MatrixNaive& source){
        if (this == &source){
            return *this;
        }
        std::memcpy(this->array, source.array, sizeof(int) * N * N);
        return *this;
    }
    float* operator[](const size_t i){
        return (array + (i * N));
    }
    float* operator[](const size_t i) const{
        return (array + (i * N));
    }
    void operator+=(const MatrixNaive& source){
        for (size_t i = 0; i < N; ++i){
            for (size_t j = 0; j < N; ++j){
                (*this)[i][j] = (*this)[i][j] + source[i][j];
            }
        }
    }
    void operator-=(const MatrixNaive& source){
        for (size_t i = 0; i < N; ++i){
            for (size_t j = 0; j < N; ++j){
                (*this)[i][j] -= source[i][j];
            }
        }
    }
    MatrixNaive operator*(const MatrixNaive& source){
        MatrixNaive temp;
        for (size_t i = 0; i < N; ++i){
            for (size_t k = 0; k < N; k++){
                for (size_t j = 0; j < N; ++j){
                    temp[i][j] += (*this)[i][k] * source[k][j];
                }
            }
        }
        return temp;
    }
    void operator/=(const float divisor){
        for (size_t i = 0; i < N; ++i){
            for (size_t j = 0; j < N; ++j){
                (*this)[i][j] /= (float)divisor;
            }
        }
    }
    float maxSumRows(){
        float maxSum = 0;
        for (size_t i = 0; i < N; ++i){
            float sum = 0;
            for (size_t j = 0; j < N; ++j){
                sum += (*this)[i][j];
            }
            if (sum > maxSum){
                maxSum = sum;
            }
        }
        return maxSum;
    }
    float maxSumColumns(){
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
    void printMatrix(){
        for (size_t i = 0; i < N; ++i){
            for (size_t j = 0; j < N; ++j){
                std::cout << array[i * N + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};
void useNaive(){
    srand(100);
    MatrixNaive A;
    MatrixNaive B;
    MatrixNaive R;
    MatrixNaive InverseMatrix;
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            float a = rand() % 100;
            A[i][j] = a;
            B[j][i] = a;
            R[i][j] = 0;
            InverseMatrix[i][j] = 0;
        }
        R[i][i] = 1;
        InverseMatrix[i][i] = 1;
    }

    B /= (A.maxSumRows() * A.maxSumColumns());
    R -= (B * A);

    MatrixNaive firstDegreeR;
    firstDegreeR = R;
    for (size_t i = 1; i < M + 1; ++i){
        InverseMatrix += R;
        R = R * firstDegreeR;
    }
    InverseMatrix = InverseMatrix * B;

    std::cout << "naive" << std::endl;
    A.printMatrix();
    InverseMatrix.printMatrix();
}

class MatrixVectorize{
private:
    float* array{};

public:
    MatrixVectorize(){
        array = new float[N * N];
        std::memset(array, 0, N * N * sizeof(float));
    }
    ~MatrixVectorize(){
        delete array;
    }
    MatrixVectorize(const MatrixVectorize& source){
        for (size_t i = 0; i < N; ++i){
            for (size_t j = 0; j < N; ++j){
                *this[i][j] = source[i][j];
            }
        }
    }

    /*
    __m512 vectorA = _mm512_loadu_ps(&(*this)[i][j]);
    __m512 vectorB = _mm512_loadu_ps(&source[i][j]);
    __m512 result = _mm512_sub_ps(vectorA, vectorB);
    _mm512_storeu_ps(&((*this)[i][j]), result);
     */

    MatrixVectorize& operator=(const MatrixVectorize& source){
        if (this == &source){
            return *this;
        }
        std::memcpy(this->array, source.array, sizeof(int) * N * N);
        return *this;
    }
    float* operator[](const size_t i){
        return (array + (i * N));
    }
    float* operator[](const size_t i) const{
        return (array + (i * N));
    }
    void operator+=(const MatrixVectorize &source){
        for (size_t i = 0; i < N; ++i){
            size_t j = 0;
/*
            if (N >= 4){
                size_t alignedSize = N - N % 4;
                for (; j < alignedSize; j += 4){
                    __m128 vectorA = _mm_loadu_ps(&(*this)[i][j]);
                    __m128 vectorB = _mm_loadu_ps(&source[i][j]);
                    __m128 result = _mm_add_ps(vectorA, vectorB);
                    _mm_storeu_ps(&((*this)[i][j]), result);
                }
            }
*/

            if (N >= 16){
                size_t alignedSize = N - N % 16;
                for (; j < alignedSize; j += 16){
                    __m512 vectorA = _mm512_loadu_ps(&(*this)[i][j]);
                    __m512 vectorB = _mm512_loadu_ps(&source[i][j]);
                    __m512 result = _mm512_add_ps(vectorA, vectorB);
                    _mm512_storeu_ps(&((*this)[i][j]), result);
                }
            }

            for (; j < N; ++i){
                (*this)[i][j] += source[i][j];
            }
        }
    }
    MatrixVectorize operator-(const MatrixVectorize& source){
        MatrixVectorize temp;
        //return temp-=source;
        for (size_t i = 0; i < N; ++i){
            size_t j = 0;
            /*
            if (N >= 4){
                size_t aligned_size = N - N % 4;
                for (; j < aligned_size; j += 4){
                    __m128 va = _mm_loadu_ps(&(*this)[i][j]);
                    __m128 vb = _mm_loadu_ps(&source[i][j]);
                    __m128 vres = _mm_sub_ps(va, vb);
                    _mm_storeu_ps(&temp[i][j], vres);
                }
            }
            */

            if (N >= 16){
                size_t alignedSize = N - N % 16;
                for (; j < alignedSize; j += 16){
                    __m512 vectorA = _mm512_loadu_ps(&(*this)[i][j]);
                    __m512 vectorB = _mm512_loadu_ps(&source[i][j]);
                    __m512 result = _mm512_sub_ps(vectorA, vectorB);
                    _mm512_storeu_ps(&((*this)[i][j]), result);
                }
            }

            for (; j < N; ++i){
                temp[i][j] = (*this)[i][j] - source[i][j];
            }
        }
        return temp;
    }
    void operator-=(const MatrixVectorize& source){
        for (size_t i = 0; i < N; ++i){
            size_t j = 0;
            /*if (N >= 4){
                size_t aligned_size = N - N % 4;
                for (; j < aligned_size; j += 4){
                    __m128 va = _mm_loadu_ps(&(*this)[i][j]);
                    __m128 vb = _mm_loadu_ps(&source[i][j]);
                    __m128 vres = _mm_sub_ps(va, vb);
                    _mm_storeu_ps(&((*this)[i][j]), vres);
                }
            }*/
            if (N >= 16){
                size_t aligned_size = N - N % 16;
                for (; j < aligned_size; j += 16){
                    /*__m128 va = _mm_loadu_ps(&(*this)[i][j]);
                    __m128 vb = _mm_loadu_ps(&source[i][j]);
                    __m128 vres = _mm_sub_ps(va, vb);
                    _mm_storeu_ps(&((*this)[i][j]), vres);*/
                    __m512 vectorA = _mm512_loadu_ps(&(*this)[i][j]);
                    __m512 vectorB = _mm512_loadu_ps(&source[i][j]);
                    __m512 result = _mm512_sub_ps(vectorA, vectorB);
                    _mm512_storeu_ps(&((*this)[i][j]), result);

                }
            }

            for (; j < N; ++i){
                (*this)[i][j] -= source[i][j];
            }
        }
    }
    MatrixVectorize operator*(const MatrixVectorize& source){
        MatrixVectorize temp;
        for (size_t i = 0; i < N; ++i){
            for (size_t k = 0; k < N; k++){
                float this_scalar = (*this)[i][k];
                size_t j = 0;
                /*
                size_t alignedSize = N - N % 4;
                if (N >= 4){
                    __m128 v_scalar =  _mm_set1_ps(this_scalar);
                    for (; j < alignedSize; j += 4){
                        __m128 v_source = _mm_loadu_ps(&source[k][j]);
                        __m128 v_mul = _mm_mul_ps(v_scalar, v_source);
                        __m128 v_temp = _mm_loadu_ps(&temp[i][j]);
                        __m128 v_res = _mm_add_ps(v_temp, v_mul);
                        _mm_storeu_ps(&temp[i][j], v_res);
                    }
                }*/
                size_t alignedSize = N - N % 16;
                if (N >= 16){
                    __m512 v_scalar =  _mm512_set1_ps(this_scalar);
                    for (; j < alignedSize; j += 16){
                        __m512 sourceVector = _mm512_loadu_ps(&source[k][j]);
                        __m512 mulVector = _mm512_mul_ps(v_scalar, sourceVector);
                        __m512 tempVector = _mm512_loadu_ps(&temp[i][j]);
                        __m512 resultVector = _mm512_add_ps(tempVector, mulVector);
                        _mm512_storeu_ps(&temp[i][j], resultVector);
                    }
                }

                for (; j < N; ++j){
                    temp[i][j] += this_scalar * source[k][j];
                }
            }
        }
        return temp;
    }
    void operator/=(const float divisor){
        for (size_t i = 0; i < N; ++i){
            size_t j = 0;
            /*
            if (N >= 4){
                size_t aligned_size = N - N % 4;
                for (; j < aligned_size; j += 4){
                    __m128 va = _mm_loadu_ps(&(*this)[i][j]);
                    __m128 vres = _mm_div_ps(va, _mm_set1_ps(divisor));
                    _mm_storeu_ps(&((*this)[i][j]), vres);
                }
            }*/

            if (N >= 16){
                size_t alignedSize = N - N % 16;
                for (; j < alignedSize; j += 16){
                    __m512 vectorA = _mm512_loadu_ps(&(*this)[i][j]);
                    __m512 result = _mm512_div_ps(vectorA, _mm512_set1_ps(divisor));
                    _mm512_storeu_ps(&((*this)[i][j]), result);
                }
            }

            for (; j < N; ++j){
                (*this)[i][j] /= divisor;
            }
        }
    }
    float maxSumRows(){
        float maxSum = 0;
        for (size_t i = 0; i < N; ++i){
            float sum = 0;
            size_t j = 0;
            if (N >= 4){
                size_t aligned_size = N - N % 4;
                for (; j < aligned_size; j += 4){
                    __m128 vector = _mm_loadu_ps((*this)[i] + j);
                    __m128 v_sum = _mm_hadd_ps(vector, vector);
                    v_sum = _mm_hadd_ps(v_sum, v_sum);
                    float res;
                    _mm_store_ss(&res, v_sum);
                    sum += res;
                }
            }
            for (; j < N; ++j){
                sum += (*this)[i][j];
            }
            if (sum > maxSum){
                maxSum = sum;
            }
        }
        return maxSum;
    }
    float maxSumColumns(){
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
    void printMatrix(){
        for (size_t i = 0; i < N; ++i){
            for (size_t j = 0; j < N; ++j){
                std::cout << array[i * N + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};
void useVectorzie(){
    srand(100);
    MatrixVectorize A;
    MatrixVectorize B;
    MatrixVectorize R;
    MatrixVectorize InverseMatrix;
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            float a = rand() % 100;
            A[i][j] = a;
            B[j][i] = a;
            R[i][j] = 0;
            InverseMatrix[i][j] = 0;
        }
        R[i][i] = 1;
        InverseMatrix[i][i] = 1;
    }

    B /= (A.maxSumRows() * A.maxSumColumns());
    R -= (B * A);

    MatrixVectorize firstDegreeR;
    firstDegreeR = R;
    for (size_t i = 1; i < M + 1; ++i){
        InverseMatrix += R;
        R = R * firstDegreeR;
    }
    InverseMatrix = InverseMatrix * B;

    std::cout << "Vect" << std::endl;
    A.printMatrix();
    InverseMatrix.printMatrix();

}

class MatrixBLAS{
private:
    float* array{};

public:
    MatrixBLAS(){
        array = new float[N * N];
        std::memset(array, 0, N * N * sizeof(float));
    }
    ~MatrixBLAS(){
        delete array;
    }
    MatrixBLAS(const MatrixBLAS &source){
        for (size_t i = 0; i < N; ++i){
            for (size_t j = 0; j < N; ++j){
                *this[i][j] = source[i][j];
            }
        }
    }


    MatrixBLAS& operator=(const MatrixBLAS& source){
        if (this == &source){
            return *this;
        }
        std::memcpy(this->array, source.array, sizeof(int) * N * N);
        return *this;
    }
    float* operator[](const size_t i){
        return (array + (i * N));
    }
    float* operator[](const size_t i) const{
        return (array + (i * N));
    }
    void operator+=(const MatrixBLAS& source){
        cblas_saxpy(N * N, 1.0, source[0], 1, (*this)[0], 1);
    }
    void operator-=(const MatrixBLAS& source){
        cblas_saxpy(N * N, -1.0, source[0], 1, (*this)[0], 1);
    }
    MatrixBLAS operator*(const MatrixBLAS& source){
        MatrixBLAS temp;
        cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans, N, N, N, 1.0, source[0], N, (*this)[0], N, 0.0, temp[0], N);
        return temp;
    }
    void operator/=(const float divisor){
        cblas_sscal(N * N, 1 / divisor, (*this)[0], 1);
    }
    float maxSumRows(){
        float maxSum = 0;
        for (size_t i = 0; i < N; ++i){
            float sum = cblas_sasum(N, (*this)[i], 1);
            if (sum > maxSum){
                maxSum = sum;
            }
        }
        return maxSum;
    }
    float maxSumColumns(){
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
    void printMatrix(){
        for (size_t i = 0; i < N; ++i){
            for (size_t j = 0; j < N; ++j){
                std::cout << array[i * N + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};
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

    MatrixBLAS firstDegreeR;
    firstDegreeR = R;
    for (size_t i = 1; i < M + 1; ++i){
        inverseMatrix += R;
        R = R * firstDegreeR;
    }
    inverseMatrix = inverseMatrix * B;

    std::cout << "blas2" << std::endl;
    A.printMatrix();
    inverseMatrix.printMatrix();
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

    return 0;
}
