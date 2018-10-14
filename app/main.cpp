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

    Matrix<5, double> m(1001);

    Block<5, double> a, b(a);

    a = b;

    return EXIT_SUCCESS;
}