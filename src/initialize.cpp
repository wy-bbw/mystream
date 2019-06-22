#include <cstdlib>
#include <algorithm>
#include <random>
#include <chrono>
#include "header.h"

void initialize_n(ARRAY_TYPE* data, long long n) {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();//seed
    std::default_random_engine dre(seed);//engine
    std::uniform_real_distribution<double> di(0.0,3.0);
    std::generate(data, data + n, [&]() {return di(dre);});
}

void initialize(ARRAY_TYPE* data) {
    initialize_n(data, getSize());
}
