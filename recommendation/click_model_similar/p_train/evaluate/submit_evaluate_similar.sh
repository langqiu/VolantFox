#!/bin/bash
source /etc/profile
cd `dirname $0`; base_dir=`pwd`
echo ${base_dir}

if [ $# -ne 4 ]; then
    echo "[ERROR] Fail to get year, month and day!"
    exit
else
    year=$1
    month=$2
    day=$3
fi

input_root_path="s3://musically-bigdata/recommend_script/data/similar_click_model_offline/similar_click_model_offline_fm_evaluate/"
auc_py=${base_dir}"/auc.py"

while true
do
    echo "pid is "$$

    input_path=${input_root_path}"dt="${year}${month}${day}"/"
    aws s3 ls ${input_path}"_SUCCESS"
    if [ $? -ne 0 ]; then
        sleep 1m
        continue
    fi
    input_path=${input_root_path}"dt="${year}${month}${day}"/*/"

    spark-submit --master yarn-cluster --conf "yarn.scheduler.maximum-allocation-mb=55g" --conf "spark.driver.maxResultSize=10g" --driver-memory 10g --num-executors 128 --name similar_evaluate --executor-memory 5g ${auc_py} ${input_path}

    new_year=`date -d "${year}${month}${day} 1 day" +"%Y"`
    new_month=`date -d "${year}${month}${day} ${hour} 1 day" +"%m"`
    new_day=`date -d "${year}${month}${day} ${hour} 1 day" +"%d"`

    year=${new_year}
    month=${new_month}
    day=${new_day}

    if [[ ${year}${month}${day} == $4 ]]; then
        break
    fi
done
