# -*- coding: utf-8 -*-

from __future__ import print_function

import sys

from pyspark import SparkContext

def parse_input(raw_data):
    try:
        # pctr, 1.0, lable, 0.0, math.abs(grad), grad * grad
        data_list = raw_data.split(' ')
        return tuple(map(float, data_list))[:6]
    except Exception:
        pass

if __name__=="__main__":
    if len(sys.argv) != 2:
        print("Usage: input path")
        exit(1)

    sc = SparkContext(appName = "evaluate")

    input_data = sc.textFile(sys.argv[1]).coalesce(2000)
    input_info = input_data.map(parse_input).filter(lambda x: x != None)

    sorted_info = input_info.sortBy(lambda x: x[0], ascending=False)

    def calPerPartition(idx, iterator):
        sumShow = 0.0
        sumClk = 0.0
        sumPositive = 0.0
        sumArea = 0.0
        sumMae = 0.0
        sumMse = 0.0
        sumPctr = 0.0
        for item in iterator:
            (pctr, show, clk, _, mae, mse) = item
            sumShow += 1
            sumClk += clk
            if clk == 0:
                sumArea += sumClk
            if clk > 0:
                sumPositive += 1
            sumMae += mae
            sumMse += mse
            sumPctr += pctr

        yield (idx, (sumShow, sumClk, sumPositive, sumPctr, sumArea, sumMae, sumMse))


    rstList = sorted_info.mapPartitionsWithIndex(calPerPartition).collect()
    rstList.sort()
    sumShow = 0.0
    sumClk = 0.0
    sumPositive = 0.0
    sumArea = 0.0
    sumMae = 0.0
    sumMse = 0.0
    sumPctr = 0.0
    for item in rstList:
        (idx, rst) = item
        (curShow, curClk, curPositive, curPctr, curArea, curMae, curMse) = rst
        sumArea += sumClk * (curShow - curPositive)
        sumShow += curShow
        sumClk += curClk
        sumPositive += curPositive
        sumPctr += curPctr
        sumArea += curArea
        sumMae += curMae
        sumMse += curMse

    pctr = sumPctr / sumShow
    rctr = sumClk / sumShow
    auc = sumArea / ((sumShow - sumPositive) * sumClk)
    mae = sumMae / sumShow
    mse = sumMse / sumShow
    nmse = mse / (rctr * (1 - rctr))

    print("show: %s, clk: %s, pctr: %s, rctr: %s, auc: %s, mae: %s, mse: %s, nmse: %s" % (sumShow,
        sumClk, pctr, rctr, auc, mae, mse, nmse))

