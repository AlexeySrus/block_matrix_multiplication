#include "block_matrix.h"

template<unsigned long block_size, typename T>
Block<block_size, T>::Block() {
    this->data = new T[block_size];
}

template<unsigned long block_size, typename T>
Block<block_size, T>::~Block() {
    delete[] this->data;
}
