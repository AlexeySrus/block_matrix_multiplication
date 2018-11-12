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
    Matrix<24, double> m("../../data/matrix1.txt", BLOCK_LINE, false);
    Matrix<24, double> m2("../../data/matrix2.txt", BLOCK_COLUMN, false);
    timer = steady_clock::now() - start_time;

    cout << "Load matrix time: " << timer.count() / 2 << " sec." << endl;

    //cout << m << endl;

    start_time = steady_clock::now();
    auto a = m*m2;
    timer = steady_clock::now() - start_time;

    cout << "Matrixes multiplication time: " << timer.count() << " sec." << endl;

    //cout << a << endl;

    return EXIT_SUCCESS;
}