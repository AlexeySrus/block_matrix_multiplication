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
    Block(const T*);
    Block(const Block<block_size, T>&);

    ~Block();

    template <unsigned long _block_size, typename _T>
    friend std::ostream& operator<<(std::ostream&, const Block<_block_size, _T>&);

    Block<block_size, T> operator=(const Block<block_size, T>&);

    template <unsigned long _block_size, typename _T>
    friend Block<_block_size, _T> operator+(const Block<_block_size, _T>&, const Block<_block_size, _T>&);

    template <unsigned long _block_size, typename _T>
    friend Block<_block_size, _T> operator*(const Block<_block_size, _T>&, const Block<_block_size, _T>&);

    void zero();
};




using namespace std;

template<unsigned long block_size, typename T>
Block<block_size, T>::Block(){
    this->data = new T[block_size*block_size];
}


template<unsigned long block_size, typename T>
Block<block_size, T>::Block(const T* V): Block(){
    memcpy(this->data, V, sizeof(T)*block_size*block_size);
}


template<unsigned long block_size, typename T>
Block<block_size, T>::Block(const std::vector<T> & v): Block(){
    //if (v.size() < block_size*block_size)
    //    throw INCORRECT_BLOCK_SIZE;

    memcpy(this->data, v.data(), sizeof(T)*block_size*block_size);
}


template<unsigned long block_size, typename T>
Block<block_size, T>::Block(const Block<block_size, T> & B) : Block(){
    memcpy(this->data, B.data, sizeof(T)*block_size*block_size);
}


template<unsigned long block_size, typename T>
Block<block_size, T>::~Block(){
    delete[] this->data;
}


template<unsigned long block_size, typename T>
std::ostream& operator<<(std::ostream & os, const Block<block_size, T> & B) {
    for (auto i = 0; i < block_size; ++i) {
        for (auto j = 0; j < block_size; ++j)
            os << B.data[i*block_size + j] << ' ';
        os << endl;
    }
    return os;
}


template<unsigned long block_size, typename T>
Block<block_size, T> Block<block_size, T>::operator=(const Block<block_size, T> & B) {
    return Block<block_size, T>(B.data);
}


template<unsigned long _block_size, typename _T>
Block<_block_size, _T> operator+(const Block<_block_size, _T> & A, const Block<_block_size, _T> & B) {
    auto res = A;
    for (auto i = 0; i < _block_size*_block_size; ++i)
        res.data[i] += B.data[i];
    return res;
}

template<unsigned long _block_size, typename _T>
Block<_block_size, _T> operator*(const Block<_block_size, _T> & A, const Block<_block_size, _T> & B) {
    Block<_block_size, _T> res;
    res.zero();

    for (auto i = 0; i < _block_size; ++i)
        for (auto j = 0; j < _block_size; ++j)
            for (auto k = 0; k < _block_size; ++k)
                res.data[i*_block_size + j] += A.data[i*_block_size + k]*B.data[k*_block_size + j];

    return res;
}

template<unsigned long block_size, typename T>
void Block<block_size, T>::zero() {
    memset(this->data, 0, sizeof(T)*block_size*block_size);
}

#endif //BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H
