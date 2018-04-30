#!/usr/bin/env python
import os,sys
for line in sys.stdin:
    sep = line.strip('\n').strip('\r\n').strip('\r').split('\t')
    if len(sep)<4:
        continue
    sMusId = sep[0]
    rMusListStr = sep[1]
    scmListStr = sep[2]
    userId = sep[3]
    rMusList = rMusListStr.split(',')
    scmList = scmListStr.split(',')
    for i in range(0, len(rMusList)):
        rMusIdInfo = rMusList[i].strip(' ')
        if len(rMusIdInfo.split(':'))<2:
            continue
        rMusId = rMusIdInfo.split(':')[1]
        scm = ''
        if i<len(scmList) and len(scmList[i].split(':'))>=2:
            scm = scmList[i].split(':')[1]
        print sMusId+'\t'+rMusId+'\t'+scm+'\t'+userId
