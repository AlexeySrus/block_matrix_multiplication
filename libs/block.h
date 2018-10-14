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


template <unsigned long block_size, typename T>
class Block{
private:
    unsigned long i, j;
    T* data;
public:
    Block();
    Block(T*);
    Block(Block<block_size, T>&);

    ~Block();

    template <unsigned long _block_size, typename _T>
    friend std::ostream& operator<<(std::ostream&, const Block<_block_size, _T>&);

    Block<block_size, T> operator=(const Block<block_size, T>&);

    void multiply(const Block<block_size, T>&, Block<block_size, T>&);
    void add(const Block<block_size, T>&, Block<block_size, T>&);

    void zero();
    std::pair<unsigned long, unsigned long> get_shift();
    void set_shift(const unsigned long, const unsigned long);
    void set_shift(const std::pair<unsigned long, unsigned long>&);
};




using namespace std;

template<unsigned long block_size, typename T>
Block<block_size, T>::Block(){
    i = 0;
    j = 0;
}


template<unsigned long block_size, typename T>
Block<block_size, T>::Block(T* V): Block(){
    this->data = V;
}


template<unsigned long block_size, typename T>
Block<block_size, T>::Block(Block<block_size, T> & B) : Block(){
    this->data = B.data;
    this->i = B.i;
    this->j = B.j;
}


template<unsigned long block_size, typename T>
Block<block_size, T>::~Block(){
    i = 0;
    j = 0;
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

template<unsigned long block_size, typename T>
void Block<block_size, T>::multiply(const Block<block_size, T> & B, Block<block_size, T> & res) {
    res.zero();

    for (auto i = 0; i < block_size; ++i)
        for (auto j = 0; j < block_size; ++j)
            for (auto k = 0; k < block_size; ++k)
                res.data[i*block_size + j] += this->data[i*block_size + k]*B.data[k*block_size + j];
}

template<unsigned long block_size, typename T>
void Block<block_size, T>::add(const Block<block_size, T> & B, Block<block_size, T> & res) {
    for (auto i = 0; i < block_size; ++i)
        for (auto j = 0; j < block_size; ++j)
            res.data[i*block_size + j] = this->data[i*block_size + j] + B.data[i*block_size + j];
}

#endif //BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H
