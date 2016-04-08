import numpy as np
n = 15
m = 10
M = np.random.choice([0,1], size = (m,n))*2-1
P =np.identity(n)+np.dot(M.T,M)
nP = [[ 7.,  2.,  0.,  0.,  6.,  2.,  0.,  0.,  6.],
     [ 2.,  7.,  0.,  0.,  2.,  6.,  0.,  0.,  2.],
     [ 0.,  0.,  7., -2.,  0.,  0.,  6., -2.,  0.],
     [ 0.,  0., -2.,  7.,  0.,  0., -2.,  6.,  0.],
     [ 6.,  2.,  0.,  0.,  7.,  2.,  0.,  0.,  6.],
     [ 2.,  6.,  0.,  0.,  2.,  7.,  0.,  0.,  2.],
     [ 0.,  0.,  6., -2.,  0.,  0.,  7., -2.,  0.],
     [ 0.,  0., -2.,  6.,  0.,  0., -2.,  7.,  0.],
     [ 6.,  2.,  0.,  0.,  6.,  2.,  0.,  0.,  7.]]

print(n)


for p in P:
    s = ""
    for n in p:
        s += str(int(n)) + " "
    s = s[:-1]
    print(s)

"""
from itertools import product
from math import e

nP = [[ 6., -3.,  3., -3.,  3.],
     [-3.,  6., -5.,  5., -5.],
     [ 3., -5.,  6., -5.,  5.],
     [-3.,  5., -5.,  6., -5.],
     [ 3., -5.,  5., -5.,  6.]]

N = 2

n = [1]

while e** -n[-1] > 0.0001:
    n = []
    for x in product(list(range(-N, N+1)), repeat = len(P)):
        n.append(sum(k[0] * k[1] for k in zip([sum(j[0] * j[1] for j in zip(i, x)) for i in P], x)))
    N += 1

print(sum(e** -i for i in n))
"""
