# -*- coding: utf-8 -*-

from __future__ import print_function

import sys, math

from pyspark import SparkContext

def predict(model, line):
    sep = line.split(' ')
    if len(sep) < 2:
        return
    label = float(sep[0])
    feature_list = [v.split(':')[0] for v in sep[1:]]
    weight_sum = [0.0]*11
    for feature in feature_list:
        print('data feature is : ' + feature)
        if feature not in model:
            continue
        weights = model[feature]
        for i in range(0,11):
            weight_sum[i] += weights[i]
    z = (-1)*weight_sum[0]
    for i in range(0,5):
        z -= weight_sum[1+i]*weight_sum[6+i]
    p = 1/(1+math.exp(z))
    grad = abs(label-p)
    output = str(p)+' 1.0 '+str(label)+' 0.0 '+str(grad)+' '+str(grad*grad)
    return output

def parseModel(line):
    sep = line.split(' ')
    if len(sep) < 14:
        return
    feature = sep[0].split(':')[0]
    params = [round(float(i), 6) for i in sep[3:14]]
    print('model feature is : ' + feature)
    return [(feature, params)]

if __name__=="__main__":
    if len(sys.argv) != 4:
        print("Usage: predict.py model_path data_path output_path")
        exit(1)

    model_path = sys.argv[1]
    data_path = sys.argv[2]
    output_path = sys.argv[3]
    sc = SparkContext(appName = 'test AUC')
    model_data = sc.textFile(model_path)
    model_info = model_data.flatMap(lambda x : parseModel(x)).filter(lambda x: x != None).collectAsMap()
    model_bc = sc.broadcast(model_info).value
    print('model dict size is : ' + str(len(model_bc)))

    test_data = sc.textFile(data_path)
    test_info = test_data.filter(lambda x: x != None).repartition(100)
    result = test_info.map(lambda x: predict(model_bc, x))
    result.saveAsTextFile(output_path)
