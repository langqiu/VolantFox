#!user/bin/env bash

start_time=`date -d "+0 day $1" +%Y%m%d`
end_time=`date -d "+0 day $2" +%Y%m%d`
command_string=''

while [ ${start_time} -le ${end_time} ]
do
	echo ${start_time}
	hive -f $3 --hiveconf day=${start_time} &
	start_time=`date -d "+1 day ${start_time}" +%Y%m%d`
done

