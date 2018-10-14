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

    template <unsigned long _block_size, typename _T>
    friend std::ostream& operator<<(std::ostream&, const Matrix<_block_size, _T>&);
};

using namespace std;

template<unsigned long block_size, typename T>
Matrix<block_size, T>::Matrix(unsigned long _n) {
    this->n = _n;
    this->data = static_cast<double*>(
                std::malloc((this->n * this->n + (this->n % (block_size*block_size)))*sizeof(T))
            );

    auto blocks_count = this->n * this->n / (block_size * block_size)
            + (this->n * this->n % (block_size * block_size) ? 1 : 0);
    blocks.resize(blocks_count);

    for (auto i = 0; i < blocks.size(); ++i)
        blocks[i] = Block<block_size, T>(data);
}

template<unsigned long block_size, typename T>
Matrix<block_size, T>::Matrix(const Matrix<block_size, T> & mat) {
    std::memcpy(this->data, mat.data, this->n*this->n*sizeof(T));
}

template<unsigned long block_size, typename T>
Matrix<block_size, T>::~Matrix() {
    free(this->data);
    this->n = 0;
}

template<unsigned long block_size, typename T>
std::ostream &operator<<(std::ostream & os, const Matrix<block_size, T> & mat) {
    for (const auto& b : mat.blocks)
        os << b << endl;
    return os;
}


#endif //BLOCK_MATRIX_MULTIPLICATION_MATRIX_H
