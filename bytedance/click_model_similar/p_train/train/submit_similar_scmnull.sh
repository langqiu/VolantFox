#!/bin/bash
source /etc/profile
cd `dirname $0`; base_dir=`pwd`
echo ${base_dir}

if [ $# -ne 4 ]; then
    echo "[ERROR] Fail to get year, month, day!"
    exit
else
    year=$1
    month=$2
    day=$3
fi

feature_root_path="s3://musically-bigdata/recommend_script/data/similar_click_model_offline/similar_click_model_offline_train_sample_scmnull/"
model_root_path="s3://musically-bigdata/recommend_script/data/similar_click_model_offline/similar_click_model_offline_fm_model_scmnull/"
evaluate_root_path="s3://musically-bigdata/recommend_script/data/similar_click_model_offline/similar_click_model_offline_fm_evaluate_scmnull/"
log_config=${base_dir}"/log4j.properties"
fm_config=${base_dir}"/config.xml"
fm_jar=${base_dir}"/Train-1.0-SNAPSHOT-jar-with-dependencies.jar"
training_set_path=${feature_root_path}"dt="${year}${month}${day}"/"

dt=`date -d "${year}${month}${day} -1 day" +"%Y%m%d"`
batch_model_path=${model_root_path}"dt="${dt}"/"
batch_model_bucket_name="musically-bigdata"
batch_model_success_file_name="recommend_script/data/similar_click_model_offline/similar_click_model_offline_fm_model_scmnull/dt="${dt}"/_SUCCESS"

output_path=${model_root_path}"dt="${year}${month}${day}"/"
aws s3 rm ${output_path} --recursive

evaluate_path=${evaluate_root_path}"dt="${year}${month}${day}"/"
aws s3 rm ${evaluate_path} --recursive

spark-submit --master yarn-cluster --conf "yarn.scheduler.maximum-allocation-mb=55g" --conf "spark.driver.maxResultSize=10g" --conf "spark.kryoserializer.buffer.max=1g" --conf "spark.driver.extraJavaOptions=-Dlog4j.configuration=log4j.properties" --conf "spark.executor.extraJavaOptions=-Dlog4j.configuration=log4j.properties" --driver-memory 10g --num-executors 100 --name train_similar --executor-memory 5g --class net.vickymedia.blueberry.model.train.Train --files ${log_config},${fm_config} ${fm_jar} ${batch_model_path} ${batch_model_bucket_name} ${batch_model_success_file_name} ${training_set_path} ${output_path} ${evaluate_path}
