# -*- coding: utf-8 -*-

from __future__ import print_function

import sys, math

from pyspark import SparkContext

def predict(model, line):
    sep = line.split('\t')
    if len(sep) < 4:
        return
    trigger_id = sep[0]
    musical_id = sep[1]
    label = float(sep[2])
    feature_str = sep[3]
    feature_list = [v.split(':')[0] for v in feature_str.split(' ')]
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
    output = trigger_id+' '+musical_id+' '+str(label)+' '+str(p)
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
    model_data_path = 's3://musically-bigdata/recommend_script/data/similar_click_model_offline/similar_click_model_offline_fm_model/dt=20171121/'
    test_data_path = 's3://musically-bigdata/recommend_script/data/similar_click_model_offline/similar_click_model_offline_hash_feature_label_daily/dt=20171121/000000_0'
    output_path = 's3://musically-bigdata/recommend_script/data/similar_click_model_offline/similar_click_model_offline_fm_evaluate_test/dt=20171121/'
    sc = SparkContext(appName = 'test AUC')
    model_data = sc.textFile(model_data_path)
    model_info = model_data.flatMap(lambda x : parseModel(x)).filter(lambda x: x != None).collectAsMap()
    model_bc = sc.broadcast(model_info).value
    print('model dict size is : ' + str(len(model_bc)))

    test_data = sc.textFile(test_data_path)
    test_info = test_data.filter(lambda x: x != None).repartition(100)
    result = test_info.map(lambda x: predict(model_bc, x))
    result.saveAsTextFile(output_path)
