#include <iostream>
#include <cmath>


int main(){

    size_t N = 1'000'000'000;
    double ln = 0;

    for (size_t n {1}; n < N; ++n){
        ln += (pow(-1, (double)(!(n % 2)))) * pow(10, (double)n) / (double)n;
    }
    //printf("%e", ln);


    return 0;
}


