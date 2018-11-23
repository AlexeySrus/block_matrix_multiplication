#ifndef BLOCK_MATRIX_MULTIPLICATION_CONFIG_H
#define BLOCK_MATRIX_MULTIPLICATION_CONFIG_H

#define BLOCK_SIZE 64
#define DATA_TYPE double

#define PARALLEL

#ifdef PARALLEL
//#define OPERATION_PARALLEL //if need block parallel, comment all line
#endif




#endif //BLOCK_MATRIX_MULTIPLICATION_CONFIG_H
