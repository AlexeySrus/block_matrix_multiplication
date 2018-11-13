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


enum BlockType{
    STANDARD,
    NON_USED,
    SYMMETRIC
};


template <unsigned long block_size, typename T> class Block{
private:
    unsigned long i, j;
    T* data;
    BlockType btype;
public:
    Block();
    Block(T *);
    Block(const Block<block_size, T> &);
    Block(T *, unsigned long, unsigned long);

    ~Block();

    template <unsigned long _block_size, typename _T>
    friend std::ostream& operator<<(std::ostream&, const Block<_block_size, _T> &);

    Block<block_size, T> operator=(const Block<block_size, T> &);

    int multiply(Block<block_size, T> &, Block<block_size, T> &);
    int add(Block<block_size, T> &, Block<block_size, T> &);

    void zero();
    const std::pair<unsigned long, unsigned long> get_shift();
    void set_shift(unsigned long, unsigned long);
    void set_shift(const std::pair<unsigned long, unsigned long> &);

    void write_by_matrix(const std::vector<std::vector<T>> &);
    void set_data(T *);
    T* get_data_pointer();
    bool used();
    void postprocess();
    void set_type(BlockType);
    const BlockType get_type();
    static void copy(Block<block_size, T> &, Block<block_size, T> &);
};


using namespace std;


template<unsigned long block_size, typename T>
Block<block_size, T>::Block(){
    i = 0;
    j = 0;
    this->btype = STANDARD;
}


template<unsigned long block_size, typename T>
Block<block_size, T>::Block(T * V): Block(){
    this->data = V;
}


template<unsigned long block_size, typename T>
Block<block_size, T>::Block(const Block<block_size, T> & B) : Block(){
    this->data = B.data;
    this->i = B.i;
    this->j = B.j;
}


template<unsigned long block_size, typename T>
Block<block_size, T>::Block(T * V, unsigned long i, unsigned long j) {
    this->data = V;
    this->i = i;
    this->j = j;
    this->btype = STANDARD;
}


template<unsigned long block_size, typename T>
Block<block_size, T>::~Block(){
    this->i = 0;
    this->j = 0;
}


template<unsigned long block_size, typename T>
std::ostream& operator<<(std::ostream & os, const Block<block_size, T> & B) {
    if (B.btype == NON_USED) {
        for (auto i = 0; i < block_size; ++i) {
            for (auto j = 0; j < block_size; ++j)
                os << 0 << ' ';
            os << endl;
        }
    }
    else if (B.btype == STANDARD){
        for (auto i = 0; i < block_size; ++i) {
            for (auto j = 0; j < block_size; ++j)
                os << B.data[i * block_size + j] << ' ';
            os << endl;
        }
    }
    else{
        for (auto i = 0; i < block_size; ++i) {
            for (auto j = 0; j < block_size; ++j)
                os << B.data[j * block_size + i] << ' ';
            os << endl;
        }
    }

    return os;
}


template<unsigned long block_size, typename T>
Block<block_size, T> Block<block_size, T>::operator=(const Block<block_size, T> & B) {
    if (&B == this)
        return *this;

    this->data = B.data;
    this->i = B.i;
    this->j = B.j;
    this->btype = B.btype;

    return *this;
}


template<unsigned long block_size, typename T>
void Block<block_size, T>::zero() {
    if (this->btype == NON_USED)
        return;
    //memset(this->data, 0, sizeof(T)*block_size*block_size);
    for (auto i = 0; i < block_size*block_size; ++i)
        this->data[i] = 0;
}


template<unsigned long block_size, typename T>
void Block<block_size, T>::set_shift(const unsigned long _i, const unsigned long _j) {
    this->i = _i;
    this->j = _j;
}


template<unsigned long block_size, typename T>
void Block<block_size, T>::set_shift(const pair<unsigned long, unsigned long> & p) {
    this->i = p.first;
    this->j = p.second;
}


template<unsigned long block_size, typename T>
const pair<unsigned long, unsigned long> Block<block_size, T>::get_shift() {
    return pair<unsigned long, unsigned long>(this->i, this->j);
}


template<unsigned long block_size, typename T>
int Block<block_size, T>::multiply(Block<block_size, T> & B, Block<block_size, T> & res) {
    if (res.get_type() == NON_USED)
        return EXIT_FAILURE;

    res.zero();

    if (this->btype == STANDARD and B.get_type() == STANDARD)
        for (auto i = 0; i < block_size; ++i)
            for (auto j = 0; j < block_size; ++j)
                for (auto k = 0; k < block_size; ++k)
                    res.data[i * block_size + j] +=
                            this->data[i * block_size + k] * B.data[k * block_size + j];
    else if (this->btype == STANDARD and B.btype == SYMMETRIC)
        for (auto i = 0; i < block_size; ++i)
            for (auto j = 0; j < block_size; ++j)
                for (auto k = 0; k < block_size; ++k)
                    res.data[i * block_size + j] +=
                            this->data[i * block_size + k] * B.data[j * block_size + k];
    else if (this->btype == SYMMETRIC and B.get_type() == STANDARD)
        for (auto i = 0; i < block_size; ++i)
            for (auto j = 0; j < block_size; ++j)
                for (auto k = 0; k < block_size; ++k)
                    res.data[i * block_size + j] +=
                            this->data[k * block_size + i] * B.data[k * block_size + j];

    return EXIT_SUCCESS;
}


template<unsigned long block_size, typename T>
int Block<block_size, T>::add(Block<block_size, T> & B, Block<block_size, T> & res) {
    if (res.get_type() == NON_USED)
        return EXIT_FAILURE;

    if (this->btype == NON_USED and B.get_type() == NON_USED){
        res.set_type(NON_USED);
        return EXIT_SUCCESS;
    }

    if (this->btype == NON_USED){
        this->copy(res, B);
        return EXIT_SUCCESS;
    }

    if (B.get_type() == NON_USED){
        this->copy(res, *this);
        return EXIT_SUCCESS;
    }

    if (this->btype == STANDARD and B.get_type() == STANDARD)
        for (auto i = 0; i < block_size; ++i)
            for (auto j = 0; j < block_size; ++j)
                res.data[i*block_size + j] = this->data[i*block_size + j] + B.data[i*block_size + j];
    else if (this->btype == STANDARD and B.btype == SYMMETRIC)
        for (auto i = 0; i < block_size; ++i)
            for (auto j = 0; j < block_size; ++j)
                res.data[i*block_size + j] = this->data[i*block_size + j] + B.data[j*block_size + i];
    else if (this->btype == SYMMETRIC and B.get_type() == STANDARD)
        for (auto i = 0; i < block_size; ++i)
            for (auto j = 0; j < block_size; ++j)
                res.data[i*block_size + j] = this->data[j*block_size + i] + B.data[i*block_size + j];

    return EXIT_SUCCESS;
}


template<unsigned long block_size, typename T>
void Block<block_size, T>::write_by_matrix(const vector<vector<T>> & matrix) {
    for (auto _i = 0; _i < block_size; ++_i)
        for (auto _j = 0; _j < block_size; ++_j)
            this->data[_i*block_size + _j] = matrix[i + _i][j + _j];
}


template<unsigned long block_size, typename T>
bool Block<block_size, T>::used() {
    return this->btype != NON_USED;
}


template<unsigned long block_size, typename T>
void Block<block_size, T>::set_data(T * V) {
    this->data = V;
}


template<unsigned long block_size, typename T>
void Block<block_size, T>::set_type(BlockType _type) {
    this->btype = _type;
}


template<unsigned long block_size, typename T>
const BlockType Block<block_size, T>::get_type() {
    return this->btype;
}

template<unsigned long block_size, typename T>
void Block<block_size, T>::postprocess() {
    if (this->btype == SYMMETRIC)
        for (auto i = 0; i < block_size; ++i)
            for (auto j = 0; j < block_size - i; ++j)
                this->data[i*block_size + j] = this->data[j*block_size + i];
}


template<unsigned long block_size, typename T>
void Block<block_size, T>::copy(Block<block_size, T> & target, Block<block_size, T> & source) {
    target.set_type(source.get_type());
    std::memcpy(target.get_data_pointer(), source.get_data_pointer(), sizeof(T)*block_size*block_size);
}

template<unsigned long block_size, typename T>
T* Block<block_size, T>::get_data_pointer() {
    return this->data;
}

#endif //BLOCK_MATRIX_MULTIPLICATION_BLOCK_MATRIX_H
