#!/user/bin/env bash

hive -f cf_map.hive --hiveconf table=$1 --hiveconf day=$2 >> log_$3
hive -f cf_precision_recall.hive --hiveconf table=$1 --hiveconf day=$2 >> log_$3
