#!/bin/bash

source /etc/profile

input_root_path="s3://musically-bigdata/recommend_script/data/similar_click_model_offline/similar_click_model_offline_fm_evaluate_test/"

input_path=$input_root_path"dt=$1/*/"

spark-submit \
	--master yarn-cluster \
	--conf "yarn.scheduler.maximum-allocation-mb=55g" \
	--conf "spark.driver.maxResultSize=10g" \
	--driver-memory 10g \
	--num-executors 128 \
	--name evaluate \
	--executor-memory 5g \
	auc.py \
	$input_path \
	> log.spark 2>&1 &
