import numpy as np
import os
import sys
import math
import tqdm
import subprocess
from config_loader import Config

config_path = '../configuration/config.h'
matrix1_path = '../data/matrix1.txt'
matrix2_path = '../data/matrix2.txt'
matrix_res_path = '../data/natrix12_res.txt'
prog_name = 'block_matrix_multiplication'
result_file = 'RESULTS.csv'

run_line = './' + prog_name + ' ' + matrix1_path + ' ' + matrix2_path + ' ' + matrix_res_path


def create_result_file(path=result_file):
    with open(path, 'w') as f:
        f.write('dtype,block_size,parallel,block_parallel,time\n')


def compile_program():
    p = subprocess.Popen('make -f Makefile', shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    retval = p.wait()


def run_program():
    out = []
    p = subprocess.Popen(run_line, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in p.stdout.readlines():
        out.append(line)
    retval = p.wait()
    return out

def run_test(block_size, dtype, parallel, use_block_parallel):
    conf = Config()
    conf.set_config(block_size, parallel, use_block_parallel, dtype)
    conf.generate_config()
    conf.save_config(config_path)

    compile_program()
    out = run_program()

    for _line in out:
        line = str(_line)
        if 'Matrixes multiplication time' in line:
            time = line.split(' ')[-2]
            with open(result_file, 'a') as f:
                f.write(
                    dtype + ',' + str(block_size) +
                    ',' + str(int(parallel)) + ','
                    + str(int(use_block_parallel)) + ','
                    + time + '\n'
                )
            break



if __name__ == '__main__':
    dtypes = ['float', 'double']

    blocks = [
        2, 4, 6, 10, 15, 20, 24, 30, 36,
        40, 60, 72, 80, 96, 120, 144, 160, 180, 240, 360, 480, 720
    ]

    parallel = [False, True]
    use_parallel_blocks = [False, True]

    create_result_file()

    total = len(dtypes)*len(blocks)*len(parallel)*len(use_parallel_blocks)

    with tqdm.tqdm(total=total) as pbar:
        for dtype in dtypes:
            for p in parallel:
                for upb in use_parallel_blocks:
                    if not p and upb:
                        pbar.update(len(blocks))
                        continue
                    for bs in blocks:
                        run_test(bs, dtype, p, upb)
                        pbar.update(1)
