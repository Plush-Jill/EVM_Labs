#include <iostream>
#include <exception>
#include <fstream>
#include <climits>



/// __builtin_ia32_rdtsc()
#define KB (int)1024
#define MB (KB * 1024)
#define L1CacheSize (48 * KB)
#define L2CacheSize ((1024 + 256) * KB)
#define L3CacheSize (12 * MB)
#define intArraySizeForL1CacheSize (L1CacheSize / (int)sizeof(int))
#define intArraySizeForL3CacheSize (L3CacheSize / (int)sizeof(int))
#define intArraySizeForKB ((int)1024 / (int)(sizeof(int)))
#define intArraySizeForMB (MB / (int)(sizeof(int)))
#define bypassCount (int)(1000 * 80000)
#define fragmentsOffsetL1 intArraySizeForL1CacheSize
#define fragmentsOffsetL3 intArraySizeForL3CacheSize
#define intArrayUniversalSize (1024 * 1024 * 16 / sizeof(int))
#define fragmentOffsetUniversal (1024 * 1024 * 16 / sizeof(int))


int main(){

    std::ofstream resultsOutput("Lab_9_bypass_results.txt", std::ios::out);
    std::ofstream trashOutput("trash_output.txt", std::ios::out);

    if (!resultsOutput.is_open()){
        throw std::exception();
    }

    int *array = new int[32 * intArraySizeForL1CacheSize];

    for (int fragmentsCount {1}; fragmentsCount < 33; ++fragmentsCount){
        int fragmentSize = intArraySizeForL1CacheSize / fragmentsCount;


        for (int i0{}; i0 < fragmentSize; ++i0){
            int k {i0};
            for (int j{1}; j < fragmentsCount; ++j){
                array[k] = k + fragmentsOffsetL1;
                k += fragmentsOffsetL1;
                if ((k % (fragmentsOffsetL1 * fragmentsCount)) < k){
                    throw std::exception();
                }
            }
            array[k] = (i0 + 1) % fragmentSize;
        }



        int temp {0};
        unsigned long long begin = __builtin_ia32_rdtsc();
        for (int i {}; i < bypassCount; ++i){
            temp = array[temp];
        }
        unsigned long long end = __builtin_ia32_rdtsc();
        trashOutput << temp + 1;

        std::string fragmentsCountInString = std::to_string(fragmentsCount);// + " fragments";
        resultsOutput << fragmentsCountInString << ';' << (end - begin) / bypassCount << std::endl;
        std::cout << fragmentsCountInString << ';' << (end - begin) / bypassCount << std::endl;

    }


    free(array);
    resultsOutput.close();
    trashOutput.close();
    return 0;
}
