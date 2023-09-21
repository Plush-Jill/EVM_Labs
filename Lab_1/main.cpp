#include <iostream>
#include <cmath>


int main(){

    size_t n = 1'000'000'000;
    double pi = 0;

    for (size_t i {}; i < n; ++i){
        pi += ((pow(-1, (double)(i % 2))) / (double)((i << 2) + 1)) * 4;
    }
    printf("%e", pi);
    return 0;
}
