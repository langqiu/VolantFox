#!/usr/bin/env python
import os,sys
user_target = ['147393646235959296', '175102384854106112']
for line in sys.stdin:
    sep = line.strip('\n').split('\t')
    from_id = sep[0]
    target = []
    for user_info in sep[1].split(';'):
        user_id = user_info.split(',')[0]
        score = float(user_info.split(',')[6])
        if score > 0.1 and user_id in user_target:
            target.append(user_info)
    if len(target)>1:
        print from_id + '\t' + ';'.join(target)
