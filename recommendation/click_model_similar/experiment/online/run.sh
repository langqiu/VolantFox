#!/usr/bin/env bash

#hive -f all_step_1.hive --hiveconf startday=$2 --hiveconf day=$1 >> log 2>&1
#hive -f all_step_3.hive --hiveconf day=$1 >> log 2>&1
#hive -f all_step_4.hive --hiveconf day=$1 >> log 2>&1

#hive -f step_1.hive --hiveconf day=$1 >> log 2>&1
#hive -f step_3.hive --hiveconf day=$1 >> log 2>&1
hive -f step_4.hive --hiveconf day=$1 >> log 2>&1
