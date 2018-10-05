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
    unsigned long i, j;
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
    friend Block<_block_size, _T> operator+(const Block<_block_size, _T>&,
                                            const Block<_block_size, _T>&);

    template <unsigned long _block_size, typename _T>
    friend Block<_block_size, _T> operator*(const Block<_block_size, _T>&,
                                            const Block<_block_size, _T>&);

    void zero();
    std::pair<unsigned long, unsigned long> get_shift();
    void set_shift(const unsigned long, const unsigned long);
    void set_shift(const std::pair<unsigned long, unsigned long>&);
};




using namespace std;

template<unsigned long block_size, typename T>
Block<block_size, T>::Block(){
    this->data = new T[block_size*block_size];
    i = 0;
    j = 0;
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
    this->i = B.i;
    this->j = B.j;
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
Block<_block_size, _T> operator+(const Block<_block_size, _T> & A,
                                 const Block<_block_size, _T> & B) {
    auto res = A;
    for (auto i = 0; i < _block_size*_block_size; ++i)
        res.data[i] += B.data[i];
    return res;
}

template<unsigned long _block_size, typename _T>
Block<_block_size, _T> operator*(const Block<_block_size, _T> & A,
                                 const Block<_block_size, _T> & B) {
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

template<unsigned long block_size, typename T>
void Block<block_size, T>::set_shift(const unsigned long i, const unsigned long j) {
    this->i = i;
    this->j = j;
}

template<unsigned long block_size, typename T>
void Block<block_size, T>::set_shift(const pair<unsigned long, unsigned long> & p) {
    this->i = p.first;
    this->j = p.second;
}

template<unsigned long block_size, typename T>
pair<unsigned long, unsigned long> Block<block_size, T>::get_shift() {
    return pair<unsigned long, unsigned long>(this->i, this->j);
}

#endif //BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H
