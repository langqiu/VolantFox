#!/usr/bin/python
import os
import sys

def hamming_bits(str1, str2):
    h1, h2 = int(str1), int(str2)
    d = h1^h2
    i = 1
    bits = ''
    while d!=0:
        if d%2!=0:
            bits = bits + str(i) + ','
        d = d/2
        i += 1
    if bits!='':
        bits = bits[0:-1]
    return bits

def hamming(str1, str2):
    h1, h2 = int(str1), int(str2)
    h, d = 0, h1^h2
    while d!=0:
        h += 1
        d &= (d-1)
    return h

if __name__ == '__main__':
    for line in sys.stdin:
        sep = line.strip().split('\t')
        if len(sep) < 4:
            continue
        bits = hamming_bits(sep[2], sep[3])
        #hamm = hamming(sep[2], sep[3])
        print sep[0] + '\t' + sep[1] + '\t' + bits
