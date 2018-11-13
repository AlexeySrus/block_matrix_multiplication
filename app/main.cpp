#include <iostream>
#include "../libs/block.h"
#include "../libs/matrix.h"
#include "../libs/logging.h"
#include <chrono>
#include <fstream>
#include "config.h"

using namespace std;
using namespace chrono;

int main(int argc, char**argv){
    if (argc < 4){
        cout << "Need 3 arguments, but input: " << argc - 1 << endl;
        cout << "Example run:" << endl;
        cout << argv[0] << "path_to matrix1 path_to_matrix2 path_to_result_matrix" << endl;
    }

    auto timer = duration<double>();

    auto start_time = steady_clock::now();
    Matrix<BLOCK_SIZE, DATA_TYPE> m(argv[1], BLOCK_LINE, false);
    Matrix<BLOCK_SIZE, DATA_TYPE> m2(argv[2], BLOCK_COLUMN, false);
    timer = steady_clock::now() - start_time;

    cout << "Average loading matrix time: " << timer.count() / 2 << " sec." << endl;

    start_time = steady_clock::now();
    auto a = m*m2;
    timer = steady_clock::now() - start_time;

    cout << "Matrixes multiplication time: " << timer.count() << " sec." << endl;

    a.savetxt(argv[3]);

    return EXIT_SUCCESS;
}