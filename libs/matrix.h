#ifndef BLOCK_MATRIX_MULTIPLICATION_MATRIX_H
#define BLOCK_MATRIX_MULTIPLICATION_MATRIX_H

#include "block.h"
#include "logging.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#define EPS 1E-5

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
    unsigned long used_blocks_count;
    unsigned long blocks_on_line;
    StorageType load_type;
    bool symmetric;
    Logger log;

    int load_matrix(const std::string&, std::vector<std::vector<T>> &);
    bool check_zero_block(Block<block_size, T> &, const std::vector<std::vector<T>> &);
public:
    Matrix();
    Matrix(const std::string&, StorageType, bool);
    Matrix(unsigned long);
    Matrix(const Matrix<block_size, T> &);
    ~Matrix();

    template <unsigned long _block_size, typename _T>
    friend std::ostream& operator<<(std::ostream&, const Matrix<_block_size, _T>&);

    template <unsigned long _block_size, typename _T>
    friend Matrix<_block_size, _T> operator*(Matrix<_block_size, _T>&, Matrix<_block_size, _T>&);

    void zero();
};


using namespace std;


template<unsigned long block_size, typename T>
Matrix<block_size, T>::Matrix(){
    this->log.set_mode(DEBUG);
    this->log.set_name("MatrixClass");
    this->log.info("Logger initialisation");
    this->symmetric = false;
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
Matrix<block_size, T>::Matrix(
        const std::string & fname, StorageType _load_type, bool _symmetric
        ) : Matrix() {
    vector<vector<T>> tmp_mat;

    if (this->load_matrix(fname, tmp_mat) != EXIT_SUCCESS) {
        this->log.error("Can't open file with name:" + fname);
        return;
    }

    this->n = tmp_mat.size();
    this->load_type = _load_type;
    this->symmetric = _symmetric;

    if (this->n % block_size){
        this->log.critical("Matrix size " + to_string(this->n) + " don't divide on block size " +
                            to_string(block_size));
        return;
    }

    this->blocks_on_line = this->n / block_size;
    auto blocks_on_line = this->blocks_on_line;

    if (_load_type == BLOCK_LINE)
        for (auto block_i = 0; block_i < blocks_on_line; ++block_i)
            for (auto block_j = 0; block_j < blocks_on_line; ++block_j)
                this->blocks.emplace_back(nullptr, block_i*block_size, block_j*block_size);
    else
        for (auto block_j = 0; block_j < blocks_on_line; ++block_j)
            for (auto block_i = 0; block_i < blocks_on_line; ++block_i)
                this->blocks.emplace_back(nullptr, block_i*block_size, block_j*block_size);

    unsigned long zero_blocks_count = 0;

    for (auto & block : this->blocks)
        if (this->check_zero_block(block, tmp_mat)) {
            block.set_type(NON_USED);
            ++zero_blocks_count;
        }

    this->used_blocks_count = this->blocks.size() - zero_blocks_count;

    this->data = new T[this->used_blocks_count*block_size*block_size];

    unsigned long tmp_shift = 0;
    for (auto i = 0; i < this->blocks.size(); ++i)
        if (this->blocks[i].used()) {
            this->blocks[i].set_data(this->data + tmp_shift*block_size*block_size);
            this->blocks[i].write_by_matrix(tmp_mat);
            tmp_shift += 1;
        }

    if (_symmetric) {
        for (auto i = 0; i < blocks_on_line; ++i) {
            this->blocks[i*blocks_on_line + i].set_type(SYMMETRIC);
            this->blocks[i*blocks_on_line + i].postprocess();
        }

        if (_load_type == BLOCK_LINE) {
            for (auto i = 0; i < blocks.size(); i += blocks_on_line) {
                this->blocks[i].set_type(SYMMETRIC);
                this->blocks[i].postprocess();
            }

            for (auto i = 0; i < blocks_on_line; ++i)
                for (auto j = i + 1; j < blocks_on_line; ++j) {
                    if (this->blocks[i * blocks_on_line + j].get_type() != NON_USED) {
                        this->log.error("Apply symmetric transform to used block");
                        return;
                    }
                    this->blocks[i * blocks_on_line + j].set_type(SYMMETRIC);
                    this->blocks[i * blocks_on_line + j].set_data(
                            this->blocks[j * blocks_on_line + i].get_data_pointer());
                }
        }
        else {
            for (auto j = 0; j < blocks_on_line; ++j)
                for (auto i = j + 1; i < blocks_on_line; ++i) {
                    if (this->blocks[i * blocks_on_line + j].get_type() != NON_USED) {
                        this->log.error("Apply symmetric transform to used block");
                        return;
                    }
                    this->blocks[i * blocks_on_line + j].set_type(SYMMETRIC);
                    this->blocks[i * blocks_on_line + j].set_data(
                            this->blocks[j * blocks_on_line + i].get_data_pointer());
                }
        }
    }
}


template<unsigned long block_size, typename T>
Matrix<block_size, T>::Matrix(const Matrix<block_size, T> & mat) : Matrix() {
    std::memcpy(this->data, mat.data, sizeof(T)*block_size*block_size*this->used_blocks_count);
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
    memset(this->data, 0, sizeof(T)*block_size*block_size*this->used_blocks_count);
}

template<unsigned long block_size, typename T>
int Matrix<block_size, T>::load_matrix(const string & fname, vector<vector<T>> & mat_vec) {
    ifstream mat_file(fname);

    if (!mat_file)
        return EXIT_FAILURE;

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

    for (auto i = 0; i < mat_vec[0].size() - 1; ++i){
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


template<unsigned long block_size, typename T>
bool Matrix<block_size, T>::check_zero_block(
        Block<block_size, T> & block,
        const vector<vector<T>> & matrix) {
    auto block_shift = block.get_shift();
    auto block_i = block_shift.first;
    auto block_j = block_shift.second;

    for (auto i = 0; i < block_size; ++i)
        for (auto j = 0; j < block_size; ++j)
            if (abs(matrix[block_i + i][block_j + j]) >= EPS)
                return false;
    return true;
}


template<unsigned long block_size, typename T>
Matrix<block_size, T> operator*(Matrix<block_size, T> & A, Matrix<block_size, T> & B) {
    auto res = Matrix<block_size, T>(A.n);
    res.zero();

    auto * tmp_data = new T[block_size*block_size];
    auto tmp_block = Block<block_size, T>(tmp_data);

    auto blocks_on_line = A.blocks_on_line;
    unsigned long block_index = 0;

    if (A.load_type == BLOCK_LINE and B.load_type == BLOCK_COLUMN)
        for (auto i = 0; i < blocks_on_line; ++i)
            for (auto j = 0; j < blocks_on_line; ++j)
                for (auto k = 0; k < blocks_on_line; ++k, ++block_index) {
                    A.blocks[i*blocks_on_line + k].multiply(B.blocks[j*blocks_on_line + k],
                                                              tmp_block);
                    res.blocks[i * blocks_on_line + j].add(tmp_block,
                                                           res.blocks[i * blocks_on_line + j]);
                }

    return res;
}

#endif //BLOCK_MATRIX_MULTIPLICATION_MATRIX_H
