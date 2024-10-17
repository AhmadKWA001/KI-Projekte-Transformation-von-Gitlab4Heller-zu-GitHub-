import numpy as np


def removeT_bett(arr: np.ndarray):
    res = arr.copy()
    res[:,1] = res[:,1] - res[:, 0]
    res[:,2] = res[:,2] - res[:, 0]
    res = res[:, 1:]
    return res