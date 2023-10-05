#include <stdio.h>
#include <math.h>


int main(){

    size_t N = 1'000'000'000;
    double ln = 0;
    double x = 10;
    for (size_t n = 0; n < N; ++n){
        ln += (pow(-1, (double)(!(n % 2)))) * pow(x, (double)n) / (double)n;
    }
    printf("%e", ln);


    return 0;
}