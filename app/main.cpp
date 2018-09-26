#include <iostream>
#include "../libs/block.h"
#include <chrono>

using namespace std;
using namespace chrono;


int main(int argc, char**argv){
    auto timer = duration<double>();

    Block<1, float> b0({1});
    Block<2, double> b2({1, 2, 3, 4});
    Block<2, double> b2_1({1, 2, 3, 4});
    Block<3, double> b3({1, 2, 3, 4, 5, 6, 7, 8, 9});

    const unsigned long N = 1024;
    Block<N, double> B;

    auto start = steady_clock::now();
    B = B * B;
    timer = steady_clock::now() - start;
    cout << timer.count() << endl;

    double *T1 = new double[N*N], *T2 = new double[N*N], *T3 = new double[N*N];

    start = steady_clock::now();
    for (auto i = 0; i < N; ++i)
        for (auto j = 0; j < N; ++j)
            for (auto k = 0; k < N; ++k)
                T3[i*N + j] += T1[i*N + k]*T2[k*N + j];
    timer = steady_clock::now() - start;
    cout << timer.count() << endl;

    return EXIT_SUCCESS;
}