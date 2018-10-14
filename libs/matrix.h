#ifndef BLOCK_MATRIX_MULTIPLICATION_MATRIX_H
#define BLOCK_MATRIX_MULTIPLICATION_MATRIX_H

#include "block.h"

template <unsigned long block_size, typename T>
class Matrix{
private:
    T* data;
    std::vector<Block<block_size, T>> blocks;
    unsigned long n;
public:
    Matrix(unsigned long);
    Matrix(const Matrix<block_size, T>&);
    ~Matrix();
};

using namespace std;

template<unsigned long block_size, typename T>
Matrix<block_size, T>::Matrix(unsigned long _n) {
    this->n = _n;
    this->data = new T[this->n * this->n + (this->n % block_size)];

    blocks.resize(this->n * this->n + (this->n % block_size) / block_size);

    for (auto i = 0; i < blocks.size(); ++i)
        blocks[i] = Block<block_size, T>(data + i*block_size);
}

template<unsigned long block_size, typename T>
Matrix<block_size, T>::Matrix(const Matrix<block_size, T> & mat) {
    std::memcpy(this->data, mat.data, this->n*this->n*sizeof(T));
}

template<unsigned long block_size, typename T>
Matrix<block_size, T>::~Matrix() {
    delete[] this->data;
    this->n = 0;
}


#endif //BLOCK_MATRIX_MULTIPLICATION_MATRIX_H
