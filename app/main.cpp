#include <iostream>
#include "../libs/block.h"
#include "../libs/matrix.h"
#include "../libs/logging.h"
#include <chrono>
#include <fstream>

using namespace std;
using namespace chrono;

int main(int argc, char**argv){
    auto timer = duration<double>();

    auto start_time = steady_clock::now();
    Matrix<2, double> m("../../data/matrix_4.txt", BLOCK_LINE, false);
    Matrix<2, double> m2("../../data/matrix_4.txt", BLOCK_COLUMN, false);
    timer = steady_clock::now() - start_time;

    cout << m << endl;

    auto a = m*m2;

    cout << "Load matrix time: " << timer.count() << " sec." << endl;

    cout << a << endl;

    return EXIT_SUCCESS;
}