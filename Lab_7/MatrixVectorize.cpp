#include "MatrixVectorize.h"






MatrixVectorize::MatrixVectorize(){
    array = new float[N * N];
    std::memset(array, 0, N * N * sizeof(float));
}
MatrixVectorize::~MatrixVectorize(){
    delete array;
}
MatrixVectorize::MatrixVectorize(const MatrixVectorize &source){
    MatrixVectorize temp;
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            temp[i][j] = source[i][j];
        }
    }
}
void MatrixVectorize::operator=(const MatrixVectorize &source){
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            (*this)[i][j] = source[i][j];
        }
    }
}
float *MatrixVectorize::operator[](const size_t i){
    return (array + (i * N));
}
float *MatrixVectorize::operator[](const size_t i) const{
    return (array + (i * N));
}
void MatrixVectorize::operator+=(const MatrixVectorize &source){
    for (size_t i = 0; i < N; ++i){
        size_t j = 0;
        if (N >= 4){
            size_t aligned_size = N - N % 4;
            for (; j < aligned_size; j += 4){
                __m128 va = _mm_loadu_ps(&(*this)[i][j]);
                __m128 vb = _mm_loadu_ps(&source[i][j]);
                __m128 vres = _mm_add_ps(va, vb);
                _mm_storeu_ps(&((*this)[i][j]), vres);
            }
        }
        for (; j < N; ++i){
            (*this)[i][j] += source[i][j];
        }
    }
}
MatrixVectorize MatrixVectorize::operator-(const MatrixVectorize &source){
    MatrixVectorize temp;
    for (size_t i = 0; i < N; ++i){
        size_t j = 0;
        if (N >= 4){
            size_t aligned_size = N - N % 4;
            for (; j < aligned_size; j += 4){
                __m128 va = _mm_loadu_ps(&(*this)[i][j]);
                __m128 vb = _mm_loadu_ps(&source[i][j]);
                __m128 vres = _mm_sub_ps(va, vb);
                _mm_storeu_ps(&temp[i][j], vres);
            }
        }
        for (; j < N; ++i){
            temp[i][j] = (*this)[i][j] - source[i][j];
        }
    }
    return temp;
}
void MatrixVectorize::operator-=(const MatrixVectorize &source){
    for (size_t i = 0; i < N; ++i){
        size_t j = 0;
        if (N >= 4){
            size_t aligned_size = N - N % 4;
            for (; j < aligned_size; j += 4){
                __m128 va = _mm_loadu_ps(&(*this)[i][j]);
                __m128 vb = _mm_loadu_ps(&source[i][j]);
                __m128 vres = _mm_sub_ps(va, vb);
                _mm_storeu_ps(&((*this)[i][j]), vres);
            }
        }
        for (; j < N; ++i){
            (*this)[i][j] -= source[i][j];
        }
    }
}
MatrixVectorize MatrixVectorize::operator*(const MatrixVectorize &source){
    MatrixVectorize temp;
    for (size_t i = 0; i < N; ++i){
        for (size_t k = 0; k < N; k++){
            float this_scalar = (*this)[i][k];
            size_t j = 0;
            size_t aligned_size = N - N % 4;
            if (N >= 4){
                __m128 v_scalar =  _mm_set1_ps(this_scalar);
                for (; j < aligned_size; j += 4){
                    __m128 v_source = _mm_loadu_ps(&source[k][j]);
                    __m128 v_mul = _mm_mul_ps(v_scalar, v_source);
                    __m128 v_temp = _mm_loadu_ps(&temp[i][j]);
                    __m128 v_res = _mm_add_ps(v_temp, v_mul);
                    _mm_storeu_ps(&temp[i][j], v_res);
                }
            }
            for (; j < N; ++j){
                temp[i][j] += this_scalar * source[k][j];
            }
        }
    }
    return temp;
}
void MatrixVectorize::operator/=(const float divisor){
    for (size_t i = 0; i < N; ++i){
        size_t j = 0;
        if (N >= 4){
            size_t aligned_size = N - N % 4;
            for (; j < aligned_size; j += 4){
                __m128 va = _mm_loadu_ps(&(*this)[i][j]);
                __m128 vres = _mm_div_ps(va, _mm_set1_ps(divisor));
                _mm_storeu_ps(&((*this)[i][j]), vres);
            }
        }
        for (; j < N; ++j){
            (*this)[i][j] /= divisor;
        }
    }
}
float MatrixVectorize::maxSumRows(){
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
float MatrixVectorize::maxSumColumns(){
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
void MatrixVectorize::printMatrix(){
    for (size_t i = 0; i < N; ++i){
        for (size_t j = 0; j < N; ++j){
            std::cout << array[i * N + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

