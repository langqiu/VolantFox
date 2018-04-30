#!/user/bin/env python
#coding = utf-8

import sys, os
import math

# merge all features
kvs = []
max_tags = 5
default_fea_value = 1.0

def appendKVS(fea_name, fea_str, fea_value):
    global kvs
    kvs.append(fea_name+':'+fea_str+''+fea_value)

def isNotValidStr(fea_str):
    return fea_str=='' or fea_str=='NULL' or fea_str=='\N'

def isNotValidList(fea_list):
    return len(fea_list)==0

def genFeature(fea, fea_name, fea_value):
    if isNotValidStr(fea):
        return
    appendKVS(fea_name, fea, str(fea_value))

def genFeatureTwoJoin(fea1, fea2, fea_name):
    if isNotValidStr(fea1) or isNotValidStr(fea2):
        return
    appendKVS(fea_name, fea1+'_'+fea2)

def genFeatureList(fea_list, fea_name, fea_value):
    if isNotValidList(fea_list):
        return
    for fea in fea_list:
        appendKVS(fea_name, fea, str(fea_value))

def genFeatureListTwoJoin(fea1_list, fea2_list, fea_name):
    if isNotValidList(fea1_list) or isNotValidList(fea2_list):
        return
    for fea1 in fea1_list:
        for fea2 in fea2_list:
            appendKVS(fea_name, fea1+'_'+fea2)

def genFeatureListFeatureJoin(fea1_list, fea2, fea_name):
    if isNotValidList(fea1_list) or isNotValidStr(fea2):
        return
    for fea1 in fea1_list:
        appendKVS(fea_name, fea1+'_'+fea2)

def parseHashTag(fea_str):
    global max_tags
    fea_list = []
    if isNotValidStr(fea_str):
        return fea_list
    for fea in fea_str.split(';'):
        if len(fea.split(','))<2:
            continue
        fea_list.append(fea.split(',')[1])
        if len(fea_list)==max_tags:
            break
    return fea_list

def parseLabel(fea_str):
    global max_tags
    fea_list = []
    if isNotValidStr(fea_str):
        return fea_list
    for fea in fea_str.split(','):
        if len(fea.split(':'))<2:
            continue
        fea_list.append(fea.split(':')[1])
        if len(fea_list)==max_tags:
            break
    return fea_list

def parseTalent(fea_str):
    global max_tags
    fea_list = []
    if isNotValidStr(fea_str):
        return fea_list
    for fea in fea_str.split(';'):
        if len(fea.split(','))<1:
            continue
        fea_list.append(fea.split(',')[1])
        if len(fea_list)==max_tags:
            break
    return fea_list

def parseIG(fea_str):
    global max_tags
    fea_list = []
    if isNotValidStr(fea_str):
        return fea_list
    for fea in fea_str.split(';'):
        if len(fea.split(','))<2:
            continue
        fea_list.append(fea.split(',')[0])
        if len(fea_list)==max_tags:
            break
    return fea_list

def processContinuousValue(fea_str, is_on):
    if isNotValidStr(fea_str):
        return ''
    if not is_on:
        return fea_str
    value = math.log(float(fea_str)+3, 2.0)
    return str(int(value))

def processNormValue(fea_str, is_on):
    if isNotValidStr(fea_str):
        return ''
    if not is_on:
        return fea_str
    return str(int(float(fea_str)*10))

for line in sys.stdin:
    sep = line.split('\t')
    s_mus_id = sep[0]
    r_mus_id = sep[1]
    view_user_id = sep[2]
    scm = sep[3]
    click = sep[4]
    s_mus_hashtags = sep[5]
    s_mus_labels = sep[6]
    s_user_id = sep[7]
    s_user_country = sep[8]
    s_user_state = sep[9]
    s_user_city = sep[10]
    s_user_age = sep[11]
    s_user_gender = sep[12]
    s_user_race = sep[13]
    s_user_c_labels = sep[14]
    r_mus_hashtags = sep[15]
    r_mus_labels = sep[16]
    r_user_id = sep[17]
    r_user_country = sep[18]
    r_user_state = sep[19]
    r_user_city = sep[20]
    r_user_age = sep[21]
    r_user_gender = sep[22]
    r_user_race = sep[23]
    r_user_c_labels = sep[24]
    r_mus_view_cnt = processContinuousValue(sep[25], False)
    r_mus_loop_cnt = processContinuousValue(sep[26], False)
    r_mus_like_cnt = processContinuousValue(sep[27], False)
    r_mus_lews_cnt = processContinuousValue(sep[28], False)
    r_mus_duration = processContinuousValue(sep[29], False)
    r_mus_loop_ratio = processNormValue(sep[30], False)
    r_mus_like_ratio = processNormValue(sep[31], False)
    r_mus_lews_ratio = processNormValue(sep[32], False)
    r_mus_q2 = processNormValue(sep[33], False)
    r_mus_q2_local = processNormValue(sep[34], False)
    r_mus_type = sep[35]
    r_mus_bright = processContinuousValue(sep[36], False)
    r_user_fans_cnt = processContinuousValue(sep[37], False)
    r_user_heart_cnt = processContinuousValue(sep[38], False)
    r_user_post_cnt = processContinuousValue(sep[39], False)
    v_user_country = sep[40]
    v_user_state = sep[41]
    v_user_city = sep[42]
    v_user_age = sep[43]
    v_user_gender = sep[44]
    v_user_race = sep[45]
    v_user_ig_tags = sep[46]
    v_user_ig_labels = sep[47]
    s_r_user_cf_score = processNormValue(sep[48], False)
    s_r_user_follow_rel = sep[49].strip()

    s_hashtag_list = parseHashTag(s_mus_hashtags)
    s_label_list = parseLabel(s_mus_labels)
    r_hashtag_list = parseHashTag(r_mus_hashtags)
    r_label_list = parseLabel(r_mus_labels)
    ig_hashtag_list = parseIG(v_user_ig_tags)
    ig_label_list = parseIG(v_user_ig_labels)
    s_user_c_labels_list = parseIG(s_user_c_labels)
    r_user_c_labels_list = parseIG(r_user_c_labels)
    
    genFeature(scm, 'scm', default_fea_value)
    # source musical feature
    genFeatureList(s_hashtag_list, 's_mus_hashtags', default_fea_value) 
    #genFeatureList(s_label_list, 's_mus_labels', default_fea_value)
    genFeature(s_user_country, 's_user_country', default_fea_value)
    #genFeature(s_user_state, 's_user_state', default_fea_value)
    #genFeature(s_user_city, 's_user_city', default_fea_value)
    genFeature(s_user_age, 's_user_age', default_fea_value)
    genFeature(s_user_gender, 's_user_gender', default_fea_value)
    genFeature(s_user_race, 's_user_race', default_fea_value)

    #genFeatureList(s_user_c_labels_list, 's_user_c_labels')
    # recommend musical feature
    genFeatureList(r_hashtag_list, 'r_mus_hashtags', default_fea_value) 
    #genFeatureList(r_label_list, 'r_mus_labels', default_fea_value) 
    genFeature(r_user_country, 'r_user_country', default_fea_value)
    #genFeature(r_user_state, 'r_user_state', default_fea_value)
    #genFeature(r_user_city, 'r_user_city', default_fea_value)
    genFeature(r_user_age, 'r_user_age', default_fea_value)
    genFeature(r_user_gender, 'r_user_gender', default_fea_value)
    genFeature(r_user_race, 'r_user_race', default_fea_value)

    #genFeatureList(r_user_c_labels_list, 'r_user_c_labels')
    # source & recommend 
    #genFeatureListTwoJoin(s_hashtag_list, r_hashtag_list, 's_r_mus_hashtags')
    #genFeatureListTwoJoin(s_label_list, r_label_list, 's_r_mus_labels')
    #genFeatureTwoJoin(s_user_country, r_user_country, 's_r_user_country')
    #genFeatureTwoJoin(s_user_state, r_user_state, 's_r_user_state')
    #genFeatureTwoJoin(s_user_city, r_user_city, 's_r_user_city')
    #genFeatureTwoJoin(s_user_age, r_user_age, 's_r_user_age')
    #genFeatureTwoJoin(s_user_gender, r_user_gender, 's_r_user_gender')
    #genFeatureTwoJoin(s_user_race, r_user_race, 's_r_user_race')
    #genFeatureTwoJoin(s_user_age+','+s_user_gender+','+s_user_race, r_user_age+','+r_user_gender+','+r_user_race, 's_r_user_age_gender_race')

    #genFeatureListTwoJoin(s_user_c_labels_list, r_user_c_labels_list, 's_r_user_c_labels')
    #genFeatureListTwoJoin(s_hashtag_list, r_label_list, 's_r_mus_hashtags_labels')

    #genFeatureListTwoJoin(s_label_list, r_hashtag_list, 's_r_mus_labels_hashtags')
    #genFeatureListFeatureJoin(s_hashtag_list, s_user_city, 's_mus_hashtag_city')
    #genFeatureListFeatureJoin(r_hashtag_list, r_user_city, 'r_mus_hashtag_city')

    # recommend musical quality
    genFeature('r_mus_view_cnt', 'r_mus_view_cnt', r_mus_view_cnt)
    genFeature('r_mus_loop_cnt', 'r_mus_loop_cnt', r_mus_loop_cnt)
    genFeature('r_mus_like_cnt', 'r_mus_like_cnt', r_mus_like_cnt)
    genFeature('r_mus_lews_cnt', 'r_mus_lews_cnt', r_mus_lews_cnt)
    genFeature('r_mus_duration', 'r_mus_duration', r_mus_duration)
    genFeature('r_mus_loop_ratio', 'r_mus_loop_ratio', r_mus_loop_ratio)
    genFeature('r_mus_like_ratio', 'r_mus_like_ratio', r_mus_like_ratio)
    genFeature('r_mus_lews_ratio', 'r_mus_lews_ratio', r_mus_lews_ratio)
    genFeature('r_mus_q2', 'r_mus_q2', r_mus_q2)
    genFeature(r_mus_type, 'r_mus_type', default_fea_value)
    genFeature('r_mus_bright', 'r_mus_bright', r_mus_bright)
    genFeature('r_user_fans_cnt', 'r_user_fans_cnt', r_user_fans_cnt)
    genFeature('r_user_heart_cnt', 'r_user_heart_cnt', r_user_heart_cnt)
    genFeature('r_user_post_cnt', 'r_user_post_cnt', r_user_post_cnt)
    # source & recommend user relation

    genFeature('s_r_user_cf_score', 's_r_user_cf_score', s_r_user_cf_score)
    genFeature(s_r_user_follow_rel, 's_r_user_follow_rel', default_fea_value)

    ## view user
    #genFeature(v_user_country, 'v_user_country')
    #genFeature(v_user_state, 'v_user_state')
    #genFeature(v_user_city, 'v_user_city')
    #genFeature(v_user_age, 'v_user_age')
    #genFeature(v_user_gender, 'v_user_gender')
    #genFeature(v_user_race, 'v_user_race')
    #genFeatureList(ig_hashtag_list, 'v_user_ig_tags')
    #genFeatureList(ig_label_list, 'v_user_ig_labels')
    ## view user & recommend musical
    #genFeatureTwoJoin(v_user_country, r_user_country, 'v_r_user_country')
    #genFeatureTwoJoin(v_user_state, r_user_state, 'v_r_user_state')
    #genFeatureTwoJoin(v_user_city, r_user_city, 'v_r_user_city')
    #genFeatureTwoJoin(v_user_age, r_user_age, 'v_r_user_age')
    #genFeatureTwoJoin(v_user_gender, r_user_gender, 'v_r_user_gender')
    #genFeatureTwoJoin(v_user_race, r_user_race, 'v_r_user_race')
    #genFeatureTwoJoin(v_user_age+','+v_user_gender+','+v_user_race, r_user_age+','+r_user_gender+','+r_user_race, 'v_r_user_age_gender_race')
    #genFeatureListTwoJoin(ig_hashtag_list, r_hashtag_list, 'v_r_hashtags')
    #genFeatureListTwoJoin(ig_label_list, r_label_list, 'v_r_labels')
    #genFeatureListTwoJoin(ig_label_list, r_user_c_labels_list, 'v_r_labels_c_labels')

    print s_mus_id+'\t'+r_mus_id+'\t'+view_user_id+'\t'+click+'\t'+chr(1).join(kvs)
    kvs = []

