#!/usr/bin/python
import os
import sys

def hamming(str1, str2):
    h1, h2 = int(str1), int(str2)
    h, d = 0, h1^h2
    while d!=0:
        h += 1
        d &= (d-1)
    return h

def hamming_bit(str1):
    h = int(str1)
    h_bit = ''
    while h!=0:
        h_bit += str(h%2)
        h /= 2
    return h_bit

if __name__ == '__main__':
    for line in sys.stdin:
        sep = line.strip().split('\t')
        if len(sep) < 4:
            continue
        print sep[0]+'\t'+sep[1]+'\t'+str(hamming(sep[2], 0))+'\t'+str(hamming(sep[3], 0))+'\t'+sep[2]+'\t'+sep[3]
        #print hamming_bit(sep[2])
