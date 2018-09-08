#ifndef BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H
#define BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H

template <unsigned long block_size, typename T> class Block{
private:
    T* data;
public:
    Block();
    ~Block();
};

#endif //BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H
