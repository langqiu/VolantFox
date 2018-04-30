#!/usr/bin/env bash

#hive -f consumer_cf_action.hive --hiveconf today=20171121

#hive -f consumer_cf_i2u.hive --hiveconf today=20171121

#hive -f consumer_cf_u2i.hive --hiveconf today=20171121

hive -f consumer_cf_i2i_rawx.hive --hiveconf today=20171121
