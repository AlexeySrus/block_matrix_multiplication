import numpy as np
import argparse
import logging
import sys
import time
import torch

logger = logging.getLogger(__name__)

def app_arguments():
    args = argparse.ArgumentParser()
    args.add_argument('matrixes', nargs='+',
                      help='Paths to matrixes.')
    args.add_argument('--mode', required=False)
    return args.parse_args()


def numpy_matrixes_multiplication(matrixes):
    res = matrixes[0].copy()

    start = time.time()
    for i in range(1, len(matrixes)):
        res = res @ matrixes[i]
    finish = time.time()

    return res, (finish - start) / (len(matrixes) - 1)


def torch_matrixes_multiplication(matrixes, device=torch.device('cpu')):
    matrix_tensors = [
        torch.from_numpy(mat).to(device).float()
        for mat in matrixes
    ]

    res = matrix_tensors[0].clone()

    start = time.time()
    for i in range(1, len(matrix_tensors)):
        res = res @ (matrix_tensors[i])
    finish = time.time()

    return res.cpu().numpy(), (finish - start) / (len(matrixes) - 1)


def main(args):
    logging.basicConfig(stream=sys.stdout, level=logging.DEBUG)

    if len(args.matrixes) < 2:
        logger.info('Don\'t input paths')
        return

    start = time.time()
    matrixes = []
    for path in args.matrixes:
        matrixes.append(np.loadtxt(path))
    finish = time.time()

    logger.info('Average loading time of one matrix: {} sec'.format(
        (finish - start) / len(matrixes))
    )

    _, numpy_time = numpy_matrixes_multiplication(matrixes)

    logger.info('Average matrix multiplication CPU time: {} sec'.format(
        numpy_time
    ))

    _, pytorch_time = torch_matrixes_multiplication(matrixes)

    logger.info('Average matrix multiplication GPU time: {} sec'.format(
        pytorch_time
    ))


if __name__ == "__main__":
    main(app_arguments())