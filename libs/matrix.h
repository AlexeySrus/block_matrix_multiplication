#ifndef BLOCK_MATRIX_MULTIPLICATION_MATRIX_H
#define BLOCK_MATRIX_MULTIPLICATION_MATRIX_H

#include "block.h"

template <unsigned long block_size, typename T>
class Matrix{
private:
    vector<Block<block_size, T>> data;
public:
    Matrix();
    Matrix(const Matrix<block_size, T>&);
};

using namespace std;

template<unsigned long block_size, typename T>
Matrix<block_size, T>::Matrix(): data() {}

template<unsigned long block_size, typename T>
Matrix<block_size, T>::Matrix(const Matrix<block_size, T> & mat) {
    this->data = mat.data;
}




#endif //BLOCK_MATRIX_MULTIPLICATION_MATRIX_H
