# -*- coding: utf-8 -*-

from __future__ import print_function

import sys, math

from pyspark import SparkContext

def parse_input(raw_data):
    try:
        data_list = raw_data.split(' ')

        key = str(data_list[0])
        key_info = key.split(':')
        sign = str(key_info[0])
        slot = str(key_info[1])

        weights = []
        for one in data_list[3:14]:
            weight = '%.8f' % float(one)
            weights.append(weight)

        return  "%s %s" % (sign, ' '.join(weights))
    except Exception:
        pass

if __name__=="__main__":
    if len(sys.argv) != 3:
       print("Usage: py, input path,output path")
       exit(1)

    input_path=sys.argv[1]
    output_path=sys.argv[2]

    sc = SparkContext(appName = 'similar model format')


    input_data = sc.textFile(input_path)
    input_info = input_data.map(parse_input).filter(lambda x: x != None)

    input_info.saveAsTextFile(output_path)
