#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char** argv){

    size_t N = (size_t)atoll(argv[1]);
    double ln = 0;
    double x = 10;

    for (size_t n = 0; n < N; ++n){
        ln += (pow(-1, (double)(!(n % 2)))) * pow(x, (double)(n)) / (double)n;
    }


    return 0;
}