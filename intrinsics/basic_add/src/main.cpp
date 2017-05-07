#include <stdlib.h>
#include <iostream>
#include <x86intrin.h>

template <typename __T, typename __U>
void printAs(__U* vec) {
    auto* resultArr = reinterpret_cast<__T*>(vec);
     for(auto index = 0u; index < 16/sizeof(__T); index++) {
        std::cout << "Result " << index << ": " << resultArr[index] << "\n";
    }
}

void float_add_shuffle() {
    float testArrA[4] = {1.1,2.2,3.3,4.4};
    float testArrB[4] = {5.1,6.2,7.3,8.4};

    auto& vectorA = *reinterpret_cast<__m128*>(testArrA);
    auto& vectorB = *reinterpret_cast<__m128*>(testArrB);

    auto result = _mm_add_ps(vectorA, vectorB);
    auto shuffle = _mm_shuffle_ps(result, result, _MM_SHUFFLE(0,1,2,3));
    _mm_storeu_ps(testArrA, shuffle);

    for(int index = 0; index < 4; index++) {
        std::cout << "Results " << index << " " << testArrA[index] << "\n";
    }
}

void int_add() {
    int32_t testArrA[4] = {10000001,10000002,10000003,10000004};
    int32_t testArrB[4] = {10000005,10000006,10000007,10000008};

    auto& vectorA = *reinterpret_cast<__m128i*>(testArrA);
    auto& vectorB = *reinterpret_cast<__m128i*>(testArrB);

    //auto result = _mm_add_epi16(vectorA, vectorB);
    auto result = _mm_add_epi32(vectorA, vectorB);

    printAs<int32_t>(&result);
}


int32_t main(int32_t argc, char *argv[]) {
    float_add_shuffle();

    return 0;
}
