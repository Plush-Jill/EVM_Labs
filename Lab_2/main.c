#include <stdio.h>
#include <math.h>

int main(){

    size_t N = 1000000000;
    double ln = 0;
    double x = 10;
    double pow1;
    double pow2;


    for (size_t n = 1; n < N; ++n){
        pow1 = pow(-1, (double)(n+1));
        pow2 = pow(x, (double)n);
        ln += pow1 * pow2 / (double)n;
    }


    return 0;
}