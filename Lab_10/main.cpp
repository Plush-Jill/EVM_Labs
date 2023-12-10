#include <iostream>
#include <fstream>
#define KB (int)1024
#define MB (KB * 1024)
#define intArraySizeForKB ((int)1024 / (int)(sizeof(int)))
#define intArraySizeForMB (MB / (int)(sizeof(int)))
#define L1CacheSize (48 * KB)
#define L2CacheSize ((1024 + 256) * KB)
#define L3CacheSize (12 * MB)
#define intArraySizeForL1CacheSize (L1CacheSize / (int)sizeof(int))
#define intArraySizeForL3CacheSize ((L3CacheSize / (int)sizeof(int)))
#define targetTestArraySize (intArraySizeForL3CacheSize * 10)
#define bypassCount (int)(1000 * 8000)
void sattoloAlgorithm(int* array, int arraySize){
    int i;
    int randomNumberBySizeModule;
    srand(unsigned(time(0)));
    for (size_t k = 0; k < arraySize; ++k){
        array[k] = k;
    }
    for(i = arraySize - 1; i > 0; --i){
        do{
            randomNumberBySizeModule = rand() % i;
        } while(randomNumberBySizeModule >= i);
        std::swap(array[i], array[randomNumberBySizeModule]);
    }
}
int main(){
    std::ofstream resultsOutput("Lab_10_results2.txt", std::ios::app);
    std::ofstream trashOutput("trash_output.txt", std::ios::out);
    if (!resultsOutput.is_open() || !trashOutput.is_open()){
        throw std::exception();
    }
    int* array {new int[targetTestArraySize]};
    sattoloAlgorithm(array, targetTestArraySize);

    for (int nopCount {}; nopCount < 2000; nopCount += 5) {
        int k{};
        /// Take beginning time here
        volatile unsigned long long begin = __builtin_ia32_rdtsc();
        for (int sizeCounter{}; sizeCounter < bypassCount; ++sizeCounter) {
            k = array[k];
            /// "nop"'s there
            for (int i{}; i < nopCount; ++i) {
                asm("nop");
            }
        }

        /// Take ending time here
        volatile unsigned long long end = __builtin_ia32_rdtsc();
        trashOutput << k;
        resultsOutput << std::to_string(nopCount) + ";" + std::to_string((end - begin) / bypassCount) << std::endl;
    }
    return 0;
}
