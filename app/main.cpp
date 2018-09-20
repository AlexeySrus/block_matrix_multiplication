#include <iostream>
#include "../libs/block.h"

using namespace std;


int main(int argc, char**argv){
    Block<2, double> b2({1, 2, 3, 4});
    Block<2, double> b2_1({1, 2, 3, 4});
    Block<3, double> b3({1, 2, 3, 4, 5, 6, 7, 8, 9});
    cout << b3 * b3 << endl;
    return EXIT_SUCCESS;
}