#include <iostream>
#include <string>
#include <random>
#include <fstream>
//#include <ctime>
#include <time.h>



/// __builtin_ia32_rdtsc()
///    printf("N = %lld, Tick = %llu\n", N*4, (end-start) / N / K);

#define KB (int)1024
#define MB (KB * 1024)
#define intArraySizeForKB ((int)1024 / (int)(sizeof(int)))
#define intArraySizeForMB (MB / (int)(sizeof(int)))
#define SizesCount (int)33
#define bypassCountPerSize (int)50

void makeForwardBypass(const int* ArraySizes){

    std::ofstream resultsOutput("forward_bypass_results.txt", std::ios::out);
    if (!resultsOutput.is_open()){
        throw std::exception();
    }

    for (int i {}; i < SizesCount; ++i){
        int currentArraySize {ArraySizes[i]};
        int* array {new int[currentArraySize]};
        for (int j {}; j < currentArraySize; ++j){
            array[j] = j + 1;
        }
        array[currentArraySize-1] = 0;


        int bypassCountPerSizeForCurrentIteration = bypassCountPerSize * (i < 10 ? i < 5 ? 50 : 5 : 1);
        if (i < 2){
            bypassCountPerSizeForCurrentIteration *= 10;
        }
        int bypassCounter {currentArraySize * bypassCountPerSizeForCurrentIteration};
        ///так называемый прогрев кэша
        for (int k {}, sizeCounter {}; sizeCounter < currentArraySize; ++sizeCounter){
            k = array[k];
        }

        /// Take beginning time here
        int k {};
        volatile unsigned long long begin = __builtin_ia32_rdtsc();
        for (int sizeCounter {}; sizeCounter < bypassCounter; ++sizeCounter){
            k = array[k];
        }
        /// Take ending time here
        volatile unsigned long long end = __builtin_ia32_rdtsc();

        std::cout << k;
        std::string arraySizeWithDimension = i < 20 ? std::to_string(currentArraySize * sizeof(int) / KB) + "KB" : std::to_string(currentArraySize * sizeof(int) / MB) + "MB";
        resultsOutput << arraySizeWithDimension << ';' << (end-begin) / currentArraySize / bypassCountPerSizeForCurrentIteration << std::endl;


    }
    resultsOutput.close();
}
void makeBackwardBypass(const int* ArraySizes){

    std::ofstream resultsOutput("backward_bypass_results.txt", std::ios::out);
    if (!resultsOutput.is_open()){
        throw std::exception();
    }

    for (int i {}; i < SizesCount; ++i){
        int currentArraySize {ArraySizes[i]};
        int* array {new int[currentArraySize]};
        for (int j {currentArraySize - 1}; j >= 0; --j){
            array[j] = j - 1;
        }
        array[0] = currentArraySize - 1;

        int bypassCountPerSizeForCurrentIteration = bypassCountPerSize * (i < 10 ? i < 5 ? 50 : 5 : 1);
        if (i < 2){
            bypassCountPerSizeForCurrentIteration *= 10;
        }
        int bypassCounter {currentArraySize * bypassCountPerSizeForCurrentIteration};

        ///так называемый прогрев кэша
        for (int k {}, sizeCounter {}; sizeCounter < currentArraySize; ++sizeCounter){
            k = array[k];
        }

        int k {};
        /// Take beginning time here
        volatile unsigned long long begin = __builtin_ia32_rdtsc();
        for (int sizeCounter {}; sizeCounter < bypassCounter; ++sizeCounter){
            k = array[k];
        }
        /// Take ending time here
        volatile unsigned long long end = __builtin_ia32_rdtsc();
        std::cout << k;

        std::string arraySizeWithDimension = i < 20 ? std::to_string(currentArraySize * sizeof(int) / KB) + "KB" : std::to_string(currentArraySize * sizeof(int) / MB) + "MB";
        resultsOutput << arraySizeWithDimension << ';' << (end-begin) / currentArraySize / bypassCountPerSizeForCurrentIteration << std::endl;

    }
    resultsOutput.close();
}
void sattoloAlgorithm(int* array, int arraySize){
    int i;
    int randomNumberBySizeModule;
    srand(unsigned(time(0)));

    for (size_t k = 0; k < arraySize; ++k) {
        array[k] = k;
    }
    for(i = arraySize - 1; i > 0; --i){
        do{
            randomNumberBySizeModule = rand() % i;
        } while(randomNumberBySizeModule >= i);
        std::swap(array[i], array[randomNumberBySizeModule]);
    }
}
void makeRandomBypass(const int* ArraySizes){

    std::ofstream resultsOutput("random_bypass_results_clear.txt", std::ios::out);
    if (!resultsOutput.is_open()){
        throw std::exception();
    }

    for (int i {}; i < SizesCount; ++i){
        int currentArraySize {ArraySizes[i]};
        int* array {new int[currentArraySize]};
        /*for (int j {}; j < currentArraySize; ++j){
            array[j] = j + 1;
        }
        array[currentArraySize-1] = 0;*/
        sattoloAlgorithm(&array[0], currentArraySize);


        ///так называемый прогрев кэша
        for (int k {}, sizeCounter {}; sizeCounter < currentArraySize; ++sizeCounter){
            k = array[k];
        }

        int bypassCountPerSizeForCurrentIteration = bypassCountPerSize * (i < 10 ? i < 5 ? 50 : 5 : 1);
        if (i < 2){
            bypassCountPerSizeForCurrentIteration *= 10;
        }
        int bypassCounter {currentArraySize * bypassCountPerSizeForCurrentIteration};
        int k {};
        /// Take beginning time here
        volatile unsigned long long begin = __builtin_ia32_rdtsc();
        for (int sizeCounter {}; sizeCounter < bypassCounter; ++sizeCounter){
            k = array[k];
        }
        /// Take ending time here
        volatile unsigned long long end = __builtin_ia32_rdtsc();
        std::cout << k;

        std::string arraySizeWithDimension = i < 20 ? std::to_string(currentArraySize * sizeof(int) / KB) + "KB" : std::to_string(currentArraySize * sizeof(int) / MB) + "MB";
        resultsOutput << arraySizeWithDimension << ';' << (end-begin) / currentArraySize / bypassCountPerSizeForCurrentIteration << std::endl;

    }
    resultsOutput.close();
}

int main(int argc, char** argv){
    int array[5];
    /*for (int j {}; j < 5; ++j){
        array[j] = j + 1;
    }*/
    //array[4] = 0;
    /*for (int i {}; i < 5; ++i){
        std::cout << array[i] << ' ';
    }*/
    //std::cout << std::endl;
    /*sattoloAlgorithm(&array[0], 5);
    for (int i {}; i < 5; ++i){
        std::cout << array[i] << ' ';
    }*/


    int ArraySizes[SizesCount];
    ArraySizes[0] = intArraySizeForKB;
    for (int i {2}; i < SizesCount; i += 2){
        ArraySizes[i] = ArraySizes[i-2] * 2;
    }
    for (int i {1}; i < SizesCount; i += 2){
        ArraySizes[i] = ArraySizes[i-1] + (ArraySizes[i-1] / 2);
    }
    /*for (int i {}; i < SizesCount; ++i) {
        std::string addd = i < 20 ? std::to_string(ArraySizes[i] * sizeof(int) / KB) + "KB" : std::to_string(ArraySizes[i] * sizeof(int) / MB) + "MB";
        std::cout << addd << std::endl;
    }*/




    if (std::string(argv[1]) == "forward"){
        makeForwardBypass(&ArraySizes[0]);
    }else if (std::string(argv[1]) == "backward"){
        makeBackwardBypass(&ArraySizes[0]);
    }else if (std::string(argv[1]) == "random"){
        makeRandomBypass(&ArraySizes[0]);
    }else{
        throw std::exception();
    }





    return 0;
}
