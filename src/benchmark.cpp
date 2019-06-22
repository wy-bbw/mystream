#include <chrono>
#include <functional>
#include "header.h"

std::chrono::nanoseconds run_benchmark(std::function<void()> bench, std::function<void()> test) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    bench();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    test();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
}
