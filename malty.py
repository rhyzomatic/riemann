from itertools import product
from math import e

P = [[ 6., -3.,  3., -3.,  3.],
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