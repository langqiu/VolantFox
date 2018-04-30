#!/bin/bash

source /etc/profile

feature_root_path="s3://musically-bigdata/recommend_script/data/similar_click_model_offline/similar_click_model_offline_train_sample/"
model_root_path="s3://musically-bigdata/recommend_script/data/similar_click_model_offline/similar_click_model_offline_fm_model/"
evaluate_root_path="s3://musically-bigdata/recommend_script/data/similar_click_model_offline/similar_click_model_offline_fm_evaluate/"

batch_model_path=$model_root_path"dt=20190101/"
batch_model_bucket_name="musically-bigdata"
batch_model_success_file_name="recommend_script/data/similar_click_model_offline/similar_click_model_offline_fm_model/dt=20190101/_SUCCESS"

training_set_path=$feature_root_path"dt=$1/"
echo "${training_set_path}"
success_file=$training_set_path"_SUCCESS"
echo "${success_file}"

output_path=$model_root_path"dt=$1/"
aws s3 rm $output_path --recursive

evaluate_path=$evaluate_root_path"dt=$1/"
aws s3 rm $evaluate_path --recursive

spark-submit \
	--master yarn-cluster \
	--conf "spark.driver.maxResultSize=45g" \
	--conf "spark.kryoserializer.buffer.max=1g" \
	--conf "spark.driver.extraJavaOptions=-Dlog4j.configuration=log4j.properties" \
	--conf "spark.executor.extraJavaOptions=-Dlog4j.configuration=log4j.properties" \
	--driver-memory 48g \
	--num-executors 200 \
	--name train_similar \
	--executor-memory 5g \
	--class net.vickymedia.blueberry.model.train.Train \
	--files log4j.properties,config.xml Train-1.0-SNAPSHOT-jar-with-dependencies.jar \
	$batch_model_path \
	$batch_model_bucket_name \
	$batch_model_success_file_name \
	$training_set_path \
	$output_path \
	$evaluate_path \
	> log.spark 2>&1 &
