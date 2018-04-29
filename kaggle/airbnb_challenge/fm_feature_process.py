#!/user/bin/env python
#coding = utf-8
import sys, os
import math
from generate_signature import generate_signature

# data field config
dim_is_requested = 0
#ds_night
#ds
id_listing_anon = 3
#id_user_anon
m_effective_daily_price =5
#m_pricing_cleaning_fee
dim_market = 7
#dim_lat
#dim_lng
#dim_room_type
#dim_person_capacity
dim_is_instant_bookable = 12
m_checkouts = 13
m_reviews = 14
days_since_last_booking = 15
#cancel_policy
image_quality_score = 17
m_total_overall_rating = 18
#m_professional_pictures
dim_has_wireless_internet = 20
#ds_night_day_of_week
#ds_night_day_of_year
ds_checkin_gap = 23
ds_checkout_gap = 24
#occ_occupancy_plus_minus_7_ds_night
#occ_occupancy_plus_minus_14_ds_night
occ_occupancy_trailing_90_ds = 27
#m_minimum_nights
#m_maximum_nights
price_booked_most_recent = 30
p2_p3_click_through_score = 31
p3_inquiry_score = 32
listing_m_listing_views_2_6_ds_night_decay = 33
#general_market_m_unique_searchers_0_6_ds_night
#general_market_m_contacts_0_6_ds_night
#general_market_m_reservation_requests_0_6_ds_night
#general_market_m_is_booked_0_6_ds_night
#m_available_listings_ds_night
kdt_score = 39
r_kdt_listing_views_0_6_avg_n100 = 40
#r_kdt_n_active_n100
#r_kdt_n_available_n100
#r_kdt_m_effective_daily_price_n100_p50
#r_kdt_m_effective_daily_price_available_n100_p50
r_kdt_m_effective_daily_price_booked_n100_p50 = 45

# outlier value config
ignore_big_outlier_map = {m_checkouts:8
                            ,m_reviews:8
                            ,days_since_last_booking:10
                            ,m_total_overall_rating:10
                            ,price_booked_most_recent:11
                            ,listing_m_listing_views_2_6_ds_night_decay:10
                            ,r_kdt_listing_views_0_6_avg_n100:5
                            ,r_kdt_m_effective_daily_price_booked_n100_p50:9}
ignore_small_outlier_map = {price_booked_most_recent:3
                            ,kdt_score:-1}
del_big_outlier_map = {m_effective_daily_price:12}
del_small_outlier_map = {m_effective_daily_price:3}

extra_feature_set = set(['fea-null', 'fea-0'])
# null value config
del_null_feature_set = set()
add_null_feature_set = set([days_since_last_booking
                        ,image_quality_score
                        ,ds_checkin_gap
                        ,ds_checkout_gap
                        ,occ_occupancy_trailing_90_ds
                        ,price_booked_most_recent
                        ,p2_p3_click_through_score
                        ,p3_inquiry_score
                        ,listing_m_listing_views_2_6_ds_night_decay
                        ,r_kdt_m_effective_daily_price_booked_n100_p50])

def gen_signature(feature_list):
    signature_list = []
    for f in feature_list:
        info = f.split(':')
        if len(info)!=2:
            continue
        f_val,f_slot = info
        sign = generate_signature(f_val, int(f_slot))
        if sign=='-1':
            continue
        signature_list.append(sign+':'+f_slot)
    return signature_list

def gen_feature(feature, slot, feature_list):
    if is_null_feature(feature):
        return
    feature_list.append(feature+':'+str(slot))

def is_null_feature(feature):
    return feature=='' or feature=='NULL' or feature=='\N'

def process_float_feature(feature, factor=1):
    if is_null_feature(feature):
        return ''
    if feature in extra_feature_set:
        return feature
    return str(int(math.log(factor*float(feature)+3, 2.0)))

def process_norm_feature(feature, factor=10):
    if is_null_feature(feature):
        return ''
    if feature in extra_feature_set:
        return feature
    return str(int(float(feature)*factor))

def process_null_value(sep):
    for i in range(0, len(sep)):
        # process not null feature
        if not is_null_feature(sep[i]):
            # occ_occupancy_trailing_90_ds process 0 value as a specific feature
            if i==occ_occupancy_trailing_90_ds and int(float(sep[i]))==0:
                sep[i] = 'fea-0'
            continue
        # process null feature
        if i in add_null_feature_set:
            sep[i] = 'fea-null'
        elif i in del_null_feature_set:
            return False
    return True

def process_outlier(feature_list):
    filter_feature_list = []
    for f in feature_list:
        info = f.split(':')
        if len(info)!=2:
            continue
        try:
            f_val = float(info[0])
        except Exception, e:
            filter_feature_list.append(f)
            continue
        f_val = float(info[0])
        f_slot = int(info[1])
        if (del_big_outlier_map.has_key(f_slot) and f_val>=del_big_outlier_map[f_slot]) \
            or (del_small_outlier_map.has_key(f_slot) and f_val<=del_small_outlier_map[f_slot]):
            return []
        if (ignore_big_outlier_map.has_key(f_slot) and f_val>=ignore_big_outlier_map[f_slot]) \
            or (ignore_small_outlier_map.has_key(f_slot) and f_val<=ignore_small_outlier_map[f_slot]):
            continue
        filter_feature_list.append(f)
    return filter_feature_list


if __name__ == '__main__':
    for line in sys.stdin:
        sep = line.strip('\n').split(',')
        if len(sep) < 46:
            continue
        # skip this data sample if return False
        if not process_null_value(sep):
            continue
        feature_list = []
        try:
            gen_feature(sep[id_listing_anon], id_listing_anon, feature_list)
            gen_feature(process_float_feature(sep[m_effective_daily_price]), m_effective_daily_price, feature_list)
            gen_feature(sep[dim_market], dim_market, feature_list)
            gen_feature(sep[dim_is_instant_bookable], dim_is_instant_bookable, feature_list)
            gen_feature(process_float_feature(sep[m_checkouts]), m_checkouts, feature_list)
            gen_feature(process_float_feature(sep[m_reviews]), m_reviews, feature_list)
            gen_feature(process_float_feature(sep[days_since_last_booking]), days_since_last_booking, feature_list)
            gen_feature(process_norm_feature(sep[image_quality_score]), image_quality_score, feature_list)
            gen_feature(process_float_feature(sep[m_total_overall_rating]), m_total_overall_rating, feature_list)
            gen_feature(sep[dim_has_wireless_internet], dim_has_wireless_internet, feature_list)
            gen_feature(sep[ds_checkin_gap], ds_checkin_gap, feature_list)
            gen_feature(sep[ds_checkout_gap], ds_checkout_gap, feature_list)
            gen_feature(process_norm_feature(sep[occ_occupancy_trailing_90_ds]), occ_occupancy_trailing_90_ds, feature_list)
            gen_feature(process_float_feature(sep[price_booked_most_recent]), price_booked_most_recent, feature_list)
            gen_feature(process_norm_feature(sep[p2_p3_click_through_score],factor=20), p2_p3_click_through_score, feature_list)
            gen_feature(process_norm_feature(sep[p3_inquiry_score],factor=20), p3_inquiry_score, feature_list)
            gen_feature(process_float_feature(sep[listing_m_listing_views_2_6_ds_night_decay], factor=100), listing_m_listing_views_2_6_ds_night_decay, feature_list)
            gen_feature(process_norm_feature(sep[kdt_score],factor=5), kdt_score, feature_list)
            gen_feature(process_float_feature(sep[r_kdt_listing_views_0_6_avg_n100]), r_kdt_listing_views_0_6_avg_n100, feature_list)
            gen_feature(process_float_feature(sep[r_kdt_m_effective_daily_price_booked_n100_p50]), r_kdt_m_effective_daily_price_booked_n100_p50, feature_list)
            # label
            label = '1' if sep[dim_is_requested]=='true' else '0'

            # process outlier and signature
            filter_feature_list = process_outlier(feature_list)
            if len(filter_feature_list)==0:
                continue
            signature_list = gen_signature(filter_feature_list)
            signature_list = sorted(signature_list, key=lambda i:int(i.split(':')[0]), reverse=False)

            # debug and output
            #print sep[0]+'\t'+sep[3]+'\t'+sep[5]+'\t'+sep[7]+'\t'+sep[12]+'\t'+sep[13]+'\t'+sep[14]+'\t'+sep[15]+'\t'+sep[17]+'\t'+sep[18]+'\t'+sep[20]+'\t'+sep[23]+'\t'+sep[24]+'\t'+sep[27]+'\t'+sep[30]+'\t'+sep[31]+'\t'+sep[32]+'\t'+sep[33]+'\t'+sep[39]+'\t'+sep[40]+'\t'+sep[45]
            #print sep[id_listing_anon]+'\t'+label+'\t'+' '.join(feature_list)
            #print sep[id_listing_anon]+'\t'+label+'\t'+' '.join(filter_feature_list)
            print label+' '+' '.join(signature_list)
        except Exception, e:
            continue
