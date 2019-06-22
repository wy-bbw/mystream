#include <cassert>
#include <cmath>
#include <iostream>
#include "header.h"


double TOL = 1.e-13;

void test(ARRAY_TYPE * a, ARRAY_TYPE *b, ARRAY_TYPE *c, ARRAY_TYPE *d) {

    for (int i = 0; i < getSize(); ++i) {
        double err = std::abs(b[i] * c[i] + d[i] - a[i]);
        assert(err < TOL);
    }
}

void test(ARRAY_TYPE * a) {
    for (int i = 0; i < 4 * getSize(); ++i) {
        double err = std::abs(a[1] * a[2] + a[3] - a[0]);
        assert(err < TOL);
    }
}

void testAdd(ARRAY_TYPE *a, ARRAY_TYPE *b, ARRAY_TYPE *c) {
    for (int i = 0; i < getSize(); ++i) {
        double err = std::abs(b[i] + c[i]  - a[i]);
        assert(err < TOL);
    }
}

long long getSize() {
    return 10000000;
}
