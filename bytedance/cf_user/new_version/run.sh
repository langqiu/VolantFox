#!/usr/bin/env python

#hive -f user_cf_i2i_action_score.hive --hiveconf today=20171121 --hiveconf yesterday=20171120
#
#hive -f uid_i2u_with_norm_weight.hive --hiveconf today=20171121
#
#hive -f uid_u2i_with_norm_weight.hive --hiveconf today=20171121

hive -f uid_wcf_i2i_to_raw_weight.hive --hiveconf today=20171121
