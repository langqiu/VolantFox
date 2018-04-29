#!/user/bin/env bash

today=$1

hive -f user_cf_action_score.hive --hiveconf day=${today} > log_cf_score
hive -f user_cf_action_score_normalization.hive --hiveconf day=${today} > log_cf_normal
hive -f uid_i2u_to_norm_weight.hive --hiveconf day=${today} > log_cf_i2u
hive -f uid_u2i_to_norm_weight.hive --hiveconf day=${today} > log_cf_u2i
hive -f uid_wcf_i2i_to_raw_weight.hive --hiveconf day=${today} > log_cf_raw

cd ../example
hive -f cf_map.hive --hiveconf table=uid_wcf_i2i_to_raw_weight --hiveconf day=${today} >> log_$2
hive -f cf_precision_recall.hive --hiveconf table=uid_wcf_i2i_to_raw_weight --hiveconf day=${today} >> log_$2
