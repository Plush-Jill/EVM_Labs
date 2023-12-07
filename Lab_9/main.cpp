#include <iostream>
#include <exception>
#include <fstream>


/// __builtin_ia32_rdtsc()
///    printf("N = %lld, Tick = %llu\n", N*4, (end-start) / N / K);

#define KB (int)1024
#define MB (KB * 1024)
#define L1CacheSize (48 * KB)
#define L2CacheSize ((1024 + 256) * KB)
#define L3CacheSize (12 * MB)
#define intArraySizeForL1CacheSize (L1CacheSize / (int)sizeof(int))
#define intArraySizeForKB ((int)1024 / (int)(sizeof(int)))
#define intArraySizeForMB (MB / (int)(sizeof(int)))
#define SizesCount (int)33
#define bypassCountPerSize (int)500


int main(){

    std::ofstream resultsOutput("Lab_9_bypass_results.txt", std::ios::out);
    if (!resultsOutput.is_open()){
        throw std::exception();
    }

    for (int fragmentsCount {1}; fragmentsCount < 33; ++fragmentsCount){
        int *array = new int[fragmentsCount * intArraySizeForL1CacheSize];
        int fragmentSize = intArraySizeForL1CacheSize / fragmentsCount;

        for (int i0{}; i0 < fragmentSize; ++i0){
            int k {i0};
            for (int j{1}; j < fragmentsCount; ++j){
                array[k] = k + intArraySizeForL1CacheSize;
                k += intArraySizeForL1CacheSize;
                if ((k % (intArraySizeForL1CacheSize * fragmentsCount)) < k){
                    throw std::exception();
                }
                //k %= (intArraySizeForL1CacheSize * fragmentsCount);
            }
            array[k] = (i0 + 1) % fragmentSize;
        }

        int bypassCountPerSizeForCurrentIteration = bypassCountPerSize * (fragmentsCount < 10 ? fragmentsCount < 5 ? 50 : 5 : 1);
        if (fragmentsCount < 4){
            bypassCountPerSizeForCurrentIteration *= 10;
        }


        int temp {0};

        unsigned long long begin = __builtin_ia32_rdtsc();
        for (int i {}; i < bypassCountPerSizeForCurrentIteration; ++i){
            temp = array[temp];
        }
        unsigned long long end = __builtin_ia32_rdtsc();
        std::cout << temp;

        std::string fragmentsCountInString = std::to_string(fragmentsCount) + " fragments";
        resultsOutput << fragmentsCountInString << ';' << (end - begin) / L1CacheSize / bypassCountPerSizeForCurrentIteration << std::endl;


    }


    return 0;
}
