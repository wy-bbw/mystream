#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include "header.h"



void initialize(ARRAY_TYPE* a);
void initialize_n(ARRAY_TYPE* a, long long n);
void test(ARRAY_TYPE * a, ARRAY_TYPE *b, ARRAY_TYPE *c, ARRAY_TYPE *d);
void testAdd(ARRAY_TYPE * a, ARRAY_TYPE *b, ARRAY_TYPE *c);
void test(ARRAY_TYPE * a);
using TIME_UNIT = std::chrono::nanoseconds;
constexpr long long TIME_CONVERSION_FACTOR = std::chrono::duration_cast<TIME_UNIT>(std::chrono::seconds(1)).count();
constexpr long long BYTE_PER_MEGABYTE = 1024 * 1024;


std::chrono::nanoseconds run_benchmark(std::function<void()> bench, std::function<void()> test);

int main() {
#pragma omp parallel
    std::cout << "testing with array of size: " << getSize() << std::endl;

    size_t size = sizeof(ARRAY_TYPE);
    ARRAY_TYPE *a = new ARRAY_TYPE[getSize()];
    ARRAY_TYPE *b = new ARRAY_TYPE[getSize()];
    ARRAY_TYPE *c = new ARRAY_TYPE[getSize()];
    ARRAY_TYPE *d = new ARRAY_TYPE[getSize()];
    ARRAY_TYPE *e = new ARRAY_TYPE[4*getSize()];
    initialize(b);
    initialize(c);
    initialize(d);
    initialize_n(e, 4*getSize());


    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    auto transferRate = [&](double duration, long scaling) {return sizeof(ARRAY_TYPE) * getSize() * scaling / duration * TIME_CONVERSION_FACTOR;};

    auto vectorTriad = [&]() {
#pragma omp for //schedule(static)
                for (int i = 0; i < getSize(); ++i) {
                    a[i] = b[i] * c[i] + d[i];
                }};
    auto triadTest = [&]() {test(a,b,c,d);};
    double duration = run_benchmark( vectorTriad, triadTest).count();
    std::cout << "a[i] = b[i] * c[i] + d[i] data transfer rate [Mb]: " << transferRate(duration, 4) / BYTE_PER_MEGABYTE << std::endl;

    auto vectorAdd = [&]() {
#pragma omp for //schedule(static)
                for (int i = 0; i < getSize(); ++i) {
                    a[i] = b[i] + c[i];
                }};
    auto addTest = [&]() {testAdd(a, b, c);};
    duration = run_benchmark(vectorAdd, addTest).count();
    std::cout << "a[i] = b[i] * c[i]  data transfer rate [Mb]: " << transferRate(duration, 3) / BYTE_PER_MEGABYTE << std::endl;


    auto oneVectorTriad = [&]() {
#pragma omp for //schedule(static)
        for (int i = 0; i < 4 * getSize(); i = i + 4) {
            e[i] = e[i+1] * e[i+2] + e[i+3];
        }
    };
    auto oneVectorTriadTest = [&](){test(e);};
    duration = run_benchmark(oneVectorTriad, oneVectorTriadTest).count();
    std::cout << "a[i] = b[i] * c[i] + d[i] data transfer rate [Mb]: " << transferRate(duration, 4) / BYTE_PER_MEGABYTE << " single vector" << std::endl;



//    begin = std::chrono::steady_clock::now();
//#pragma omp for //schedule(static)
//    for (int i = 0; i < 4 * getSize(); i = i + 4) {
//        e[i] = e[i+1] * e[i+2] + e[i+3];
//    }
//    end = std::chrono::steady_clock::now();
//    test(e);
//    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
//    transferRate = sizeof(ARRAY_TYPE) * getSize() * 4 / duration * TIME_CONVERSION_FACTOR; 
//    std::cout << "a[i] = b[i] * c[i] + d[i] data transfer rate [Mb]: " << transferRate / BYTE_PER_MEGABYTE  << " single array "<< std::endl;


    delete[] a;
    delete[] b;
    delete[] c;
    delete[] d;
    delete[] e;
}
