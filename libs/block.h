#ifndef BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H
#define BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H

#include <vector>
#include <iostream>
#include <cstdint>
#include <cstring>

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
    template <unsigned long _block_size, typename _T>
    friend std::ostream& operator<<(std::ostream&, const Block<_block_size, _T>&);
};




using namespace std;

template<unsigned long block_size, typename T>
Block<block_size, T>::Block(){
    this->data = new T[block_size*block_size];
}

template<unsigned long block_size, typename T>
Block<block_size, T>::~Block(){
    delete[] this->data;
}

template<unsigned long block_size, typename T>
Block<block_size, T>::Block(const std::vector<T> & v): Block(){
    //if (v.size() < block_size*block_size)
    //    throw INCORRECT_BLOCK_SIZE;

    memcpy(this->data, v.data(), sizeof(T)*block_size*block_size);
}

template<unsigned long block_size, typename T>
std::ostream& operator<<(std::ostream & os, const Block<block_size, T> & b) {
    for (auto i = 0; i < block_size*block_size - 1; ++i)
        os << b.data[i] << ' ';
    os << b.data[block_size*block_size - 1];
    return os;
}

#endif //BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H
