#!/usr/bin/env bash
cd `dirname $0`; base_dir=`pwd`

model_path="s3://musically-bigdata/recommend_script/data/wei_hu_copy/booking_predict/booking_predict_original_dataset/fm_model/$1/"
data_path="s3://musically-bigdata/recommend_script/data/wei_hu_copy/booking_predict/booking_predict_original_dataset/fm_feature/$1/$2/"
output_path="s3://musically-bigdata/recommend_script/data/wei_hu_copy/booking_predict/booking_predict_original_dataset/fm_evaluate/$1/$3/"
aws s3 rm ${output_path} --recursive
spark-submit \
	--conf "spark.yarn.executor.memoryOverhead=5120" \
        --conf "spark.driver.maxResultSize=48g" \
        --driver-memory 45g \
        --num-executors 10 \
        --name booking_predict \
        --executor-memory 5g \
        ${base_dir}/predict.py ${model_path} ${data_path} ${output_path} \
        > log.spark 2>&1
