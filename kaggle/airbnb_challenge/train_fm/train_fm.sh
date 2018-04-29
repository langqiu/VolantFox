#!/bin/bash
source /etc/profile
cd `dirname $0`; base_dir=`pwd`
echo ${base_dir}

if [ $# -ne 1 ]; then
    echo "[ERROR] Fail to get year, month, day!"
    exit
else
    cross_validation=$1
fi

feature_root_path="s3://musically-bigdata/recommend_script/data/wei_hu_copy/booking_predict/booking_predict_original_dataset/fm_feature/"
model_root_path="s3://musically-bigdata/recommend_script/data/wei_hu_copy/booking_predict/booking_predict_original_dataset/fm_model/"
evaluate_root_path="s3://musically-bigdata/recommend_script/data/wei_hu_copy/booking_predict/booking_predict_original_dataset/fm_evaluate/"
log_config=${base_dir}"/log4j.properties"
fm_config=${base_dir}"/config.xml"
fm_jar=${base_dir}"/Train-1.0-SNAPSHOT-jar-with-dependencies.jar"

batch_model_path=${model_root_path}"error/"
echo ${batch_model_path}
batch_model_bucket_name="musically-bigdata"
echo ${batch_model_bucket_name}
batch_model_success_file_name="recommend_script/data/wei_hu_copy/booking_predict/booking_predict_original_dataset/fm_model/error/_SUCCESS"
echo ${batch_model_success_file_name}

training_set_path=${feature_root_path}"${cross_validation}/training_set/"
echo ${training_set_path}
output_path=${model_root_path}"${cross_validation}/"
echo ${output_path}
aws s3 rm ${output_path} --recursive
evaluate_path=${evaluate_root_path}"${cross_validation}/"
echo ${evaluate_path}
aws s3 rm ${evaluate_path} --recursive

spark-submit --conf "yarn.scheduler.maximum-allocation-mb=55g" --conf "spark.driver.maxResultSize=10g" --conf "spark.kryoserializer.buffer.max=1g" --conf "spark.driver.extraJavaOptions=-Dlog4j.configuration=log4j.properties" --conf "spark.executor.extraJavaOptions=-Dlog4j.configuration=log4j.properties" --driver-memory 10g --num-executors 10 --name train_booking --executor-memory 5g --class net.vickymedia.blueberry.model.train.Train --files ${log_config},${fm_config} ${fm_jar} ${batch_model_path} ${batch_model_bucket_name} ${batch_model_success_file_name} ${training_set_path} ${output_path} ${evaluate_path}
