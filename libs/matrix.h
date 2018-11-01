#ifndef BLOCK_MATRIX_MULTIPLICATION_MATRIX_H
#define BLOCK_MATRIX_MULTIPLICATION_MATRIX_H

#include "block.h"
#include "logging.h"

#include <iostream>
#include <string>
#include <fstream>

enum StorageType{
    BLOCK_LINE,
    BLOCK_COLUMN
};

void split(std::string str, std::string splitBy, std::vector<std::string>& tokens);

template <unsigned long block_size, typename T>
class Matrix{
private:
    T* data;
    std::vector<Block<block_size, T>> blocks;
    unsigned long n;
    Logger log;

    int load_matrix(const std::string&, std::vector<std::vector<T>> &);
public:
    Matrix();
    Matrix(const std::string&, StorageType);
    Matrix(unsigned long);
    Matrix(const Matrix<block_size, T>&);
    ~Matrix();

    template <unsigned long _block_size, typename _T>
    friend std::ostream& operator<<(std::ostream&, const Matrix<_block_size, _T>&);

    void zero();
};


using namespace std;


template<unsigned long block_size, typename T>
Matrix<block_size, T>::Matrix(){
    this->log.set_mode(DEBUG);
    this->log.set_name("MatrixClass");
    this->log.info("Logger initialisation");
};


template<unsigned long block_size, typename T>
Matrix<block_size, T>::Matrix(unsigned long n) : Matrix() {
    this->n = n;

    auto blocks_count = this->n * this->n / (block_size * block_size)
            + (this->n * this->n % (block_size * block_size) ? 1 : 0);

    this->data = new T[blocks_count*block_size*block_size];

    this->blocks.resize(blocks_count);

    for (auto i = 0; i < this->blocks.size(); ++i)
        this->blocks[i] = Block<block_size, T>(data + i*block_size*block_size);

    this->log.debug("Matrix allocate");
}


template<unsigned long block_size, typename T>
Matrix<block_size, T>::Matrix(const std::string & fname, StorageType load_type) : Matrix() {
    vector<vector<T>> tmp_mat;

    if (this->load_matrix(fname, tmp_mat) != EXIT_SUCCESS)
        return;
}


template<unsigned long block_size, typename T>
Matrix<block_size, T>::Matrix(const Matrix<block_size, T> & mat) : Matrix() {
    std::memcpy(this->data, mat.data, sizeof(T)*block_size*block_size*this->blocks.size());
    this->n = mat.n;
    this->blocks = mat.blocks;
}


template<unsigned long block_size, typename T>
Matrix<block_size, T>::~Matrix() {
    delete[] this->data;
    this->n = 0;
}


template<unsigned long block_size, typename T>
std::ostream &operator<<(std::ostream & os, const Matrix<block_size, T> & mat) {
    for (const auto& b : mat.blocks)
        os << b << endl;
    return os;
}


template<unsigned long block_size, typename T>
void Matrix<block_size, T>::zero() {
    memset(this->data, 0, sizeof(T)*block_size*block_size*this->blocks.size());
}

template<unsigned long block_size, typename T>
int Matrix<block_size, T>::load_matrix(const string & fname, vector<vector<T>> & mat_vec) {
    ifstream mat_file(fname);

    if (!mat_file){
        this->log.error("Can't open file with name:" + fname);
        return EXIT_FAILURE;
    }

    vector<string> vec_line;
    string line;
    vector<T> tmp_line_values;

    getline(mat_file, line);
    split(line, " ", vec_line);

    for (auto j = 0; j < vec_line.size(); ++j)
        tmp_line_values.push_back(stod(vec_line[j]));

    mat_vec.push_back(tmp_line_values);
    vec_line.clear();
    tmp_line_values.clear();

    for (auto i = 0; i < this->n - 1; ++i){
        getline(mat_file, line);
        split(line, " ", vec_line);

        for (auto j = 0; j < vec_line.size(); ++j)
            tmp_line_values.push_back(stod(vec_line[j]));

        mat_vec.push_back(tmp_line_values);
        vec_line.clear();
        tmp_line_values.clear();
    }

    mat_file.close();

    return EXIT_SUCCESS;
}


#endif //BLOCK_MATRIX_MULTIPLICATION_MATRIX_H
