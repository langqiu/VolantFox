#!/usr/bin/env bash
cd `dirname $0`; base_dir=`pwd`

spark-submit \
	--conf "spark.yarn.executor.memoryOverhead=5120" \
        --conf "spark.driver.maxResultSize=48g" \
        --driver-memory 45g \
        --num-executors 100 \
        --name similar_predict \
        --executor-memory 5g \
        ${base_dir}/$1 \
        > log.spark 2>&1 &
