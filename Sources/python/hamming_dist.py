import numpy as np
from scipy.spatial import distance

def hamming_distance(a, b):
    r = (1 << np.arange(8))[:,None]
    return np.count_nonzero( (a & r) != (b & r) )

u = np.array([[0], [0x1F], [0xE3]], dtype=np.uint8)
ham_dist = distance.hamming(u[0], u[1])
print(ham_dist)

to_test = np.arange(0, 255, dtype=np.uint8)

to_test = [ np.array([num], dtype=np.uint8) for num in to_test]
to_test = np.array(to_test)

a = np.unpackbits(to_test, axis=1)

last_num = a[0]
ham_list = [a[0]]

for num in a:
    ham_dist = hamming_distance(last_num, num)
    if(ham_dist == 5):
        ham_list.append(num)
        last_num = num

ham_list = np.packbits(ham_list)
ham_list = np.array([hex(num) for num in ham_list])
print(ham_list)