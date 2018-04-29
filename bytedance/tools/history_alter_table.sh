#!user/bin/env bash

start_time=`date -d "+0 day $1" +%Y%m%d`
end_time=`date -d "+0 day $2" +%Y%m%d`
command_string=''

while [ ${start_time} -le ${end_time} ]
do
        echo ${start_time}
        last_string=${command_string}
        curr_string="alter table $3 add if not exists partition(dt=${start_time}, obj='COMMENT', type='COMMENT_CREATED');"
        command_string=${last_string}${curr_string}
        start_time=`date -d "+1 day ${start_time}" +%Y%m%d`
done

#echo ${command_string}
hive -e "${command_string}"
