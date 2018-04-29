#!/usr/bin/env python
import os,sys
for line in sys.stdin:
    sep = line.strip().split('\t')
    v_name_len = len(sep[3])
    name_len = len(sep[1])
    len_rate = round(v_name_len*100.0/name_len, 2)
    print line.strip() + '\t' + str(v_name_len) + '\t' + str(len_rate)
