#!/usr/bin/env python
import os,sys
a = 0
b = 0
for line in sys.stdin:
    sep = line.strip().split('\t')
    if sep[0]=='NULL':
        print sep[0]
        continue
    a += int(sep[1])
    b += int(sep[2])
print a*1.0/b
