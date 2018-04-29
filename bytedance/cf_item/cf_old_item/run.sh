#!/usr/bin/env python

#hive -f uid2i_action_table_for_i2i.hive --hiveconf today=20171024 --hiveconf yesterday=20171010 --hiveconf startday=20171010 --hiveconf endday=20171024 > log1 2>&1
#hive -f uid2i_action_table_for_i2i.hive --hiveconf today=20171108 --hiveconf yesterday=20171024 --hiveconf startday=20171025 --hiveconf endday=20171108 > log1 2>&1
#hive -f musical_i2u_with_norm.hive --hiveconf day=20171108 > log2 2>&1
#hive -f musical_u2i_with_norm.hive --hiveconf day=20171108 > log3 2>&1
hive -f musical_cf_i2i_rawx.hive --hiveconf day=20171108 > log4 2>&1
