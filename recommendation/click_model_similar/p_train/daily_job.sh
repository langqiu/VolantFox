#!/bin/bash
source /etc/profile
cd `dirname $0`; base_dir=`pwd`
echo ${base_dir}

while true
do
    year=`date +%Y`
    month=`date +%m`
    day=`date +%d`
    pre_year=`date -d "1 days ago" +%Y`
    pre_month=`date -d "1 days ago" +%m`
    pre_day=`date -d "1 days ago" +%d`

    hour=`date +%H`
    if [ $hour -eq 3 ]
    then
        echo "时间到达开始处理任务..."
        nohup sh /mnt/wei.hu/click_model_similar/p_train/train/submit_similar.sh $pre_year $pre_month $pre_day $year$month$day >> /mnt/wei.hu/click_model_similar/p_train/train/train.log 2>&1 &

        nohup sh /mnt/wei.hu/click_model_similar/p_train/evaluate/submit_evaluate_similar.sh $pre_year $pre_month $pre_day $year$month$day >> /mnt/wei.hu/click_model_similar/p_train/evaluate/evaluate.log 2>&1 &

        echo "处理任务... date is $year$month$day$hour"
        sleep 1h
    else
        echo "等待时间到达... date is $year$month$day$hour"
        sleep 1h
    fi
done
