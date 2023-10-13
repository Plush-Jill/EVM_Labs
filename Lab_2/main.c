#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char** argv){

    size_t N = (size_t)atoll(argv[1]);
    double ln = 0;
    double x = 10;
    double pow1;
    double pow2;


    for (size_t n = 0; n < N; ++n){
        pow1 = pow(-1, (double)(!(n%2)));
        pow2 = pow(x, (double)n);
        ln += pow1 * pow2 / (double)n;
    }


    return 0;
}