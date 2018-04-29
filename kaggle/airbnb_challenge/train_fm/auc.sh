#!/bin/bash
source /etc/profile

input_path="s3://musically-bigdata/recommend_script/data/wei_hu_copy/booking_predict/booking_predict_original_dataset/fm_evaluate/$1/$2/"
spark-submit \
	--master yarn-cluster \
	--conf "yarn.scheduler.maximum-allocation-mb=55g" \
	--conf "spark.driver.maxResultSize=10g" \
	--driver-memory 10g \
	--num-executors 10 \
	--name evaluate \
	--executor-memory 5g \
	auc.py \
	$input_path \
	> log.spark 2>&1
