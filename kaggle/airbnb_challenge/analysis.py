#!/usr/bin/env python
#coding = utf-8
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import sys
ROOT_PATH = '/Users/weihu/work_dir/airbnb_challenge/tmp/'

def load_csv(data_path):
    data = pd.read_csv(data_path)
    # remove duplicate and null
    data = data.drop_duplicates().dropna(how='all')
    return data

def get_cols(data):
    obj_cols = []
    int_cols = []
    float_cols = []
    cols = data.columns.tolist()
    for col in cols:
        if data[col].dtype=='object':
            obj_cols.append(col)
        elif data[col].dtype=='int64':
            int_cols.append(col)
        elif data[col].dtype=='float64':
            float_cols.append(col)
    return cols, obj_cols, int_cols, float_cols

def write_cols_type(obj_cols, int_cols, float_cols):
    pfile = open(ROOT_PATH+'feature_type.txt', 'w')
    pfile.write('########## object ##########\n')
    pfile.write(str(obj_cols)+'\n')
    pfile.write('########## int ##########\n')
    pfile.write(str(int_cols)+'\n')
    pfile.write('########## float ##########\n')
    pfile.write(str(float_cols)+'\n')
    pfile.close()

def cal_miss(data, cols):
    miss_info = []
    for col in cols:
        miss_count = data.shape[0] - data[col].count()
        miss_percent = float(miss_count) /  data.shape[0] * 100
        miss_info.append(col+':'+str(miss_percent))
    miss_info = sorted(miss_info, key=lambda i:float(i.split(':')[1]), reverse=True)
    pfile = open(ROOT_PATH+'feature_miss_percentage.txt', 'w')
    for info in miss_info:
        pfile.write(info+'\n')
    pfile.close()

def cal_hist(data, cols):
    for col in cols:
        try:
            data[col].dropna().hist(bins=128)
            plt.savefig(ROOT_PATH+'hist_pic/'+col+'.png')
            plt.clf()
        except Exception, e:
            continue

def cal_feature(data, cols):
    pfile = open(ROOT_PATH+'feature_statistics.txt', 'w')
    for col in cols:
        try:
            pfile.write('########## '+col+' ##########\n')
            info = str(data[col].dropna().describe())
            pfile.write(info+'\n')
        except Exception, e:
            continue
    pfile.close()

if __name__ == '__main__':
    data_path = './data/TH_data_challenge.csv'
    data = load_csv(data_path)
    cols,obj_cols,int_cols,float_cols = get_cols(data)
    write_cols_type(obj_cols, int_cols, float_cols)
    cal_miss(data, cols)
    cal_hist(data, float_cols)
    cal_feature(data, float_cols)

#from sklearn.preprocessing import StandardScaler
#from sklearn.feature_selection import SelectKBest
#from sklearn.feature_selection import mutual_info_classif
#data.m_effective_daily_price.plot(kind='density')
#data.dim_is_requested.unique()
