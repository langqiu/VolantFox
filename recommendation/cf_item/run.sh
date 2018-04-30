#!/usr/bin/env python

#hive -f action_fromu2i_for_i2i_upgrade.hive --hiveconf today=20171113 --hiveconf yesterday=20170913 --hiveconf startday=20170914 --hiveconf endday=20171113 > log1 2>&1
#hive -f musical_i2u_with_norm.hive --hiveconf day=20171213 > log2 2>&1
#hive -f musical_u2i_with_norm_country.hive --hiveconf day=20171213 > log 2>&1
hive -f musical_cf_i2i_rawx_country.hive --hiveconf day=20171213 > log 2>&1
