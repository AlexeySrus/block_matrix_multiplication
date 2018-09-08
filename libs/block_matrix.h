#ifndef BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H
#define BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H

#include <vector>
#include <iostream>

/*
 * Exceptions block
 */

#define INCORRECT_BLOCK_SIZE 1001


template <unsigned long block_size, typename T> class Block{
private:
    T* data;
public:
    Block();
    Block(const std::vector<T>&);
    ~Block();
};

#endif //BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H
