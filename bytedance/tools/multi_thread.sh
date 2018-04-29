#!/usr/bin/bash
cd `dirname $0`; base_dir=`pwd`

if [ $# -ne 4 ]; then
	echo "usage : sh $0 data_path parallel_number output_path script"
	echo "merge all the data in data_path and resplit them into parallel_number parts, then process these data by the script and output into output_path"
	exit;
fi
data_dir=$1
parallel_number=$2
output_path=$3
script=$4
cd ${data_dir}

# create file to collect data
target_file_name='.part'
if [ -f ${target_file_name} ]; then
	echo "error : file exists! ${target_file_name}"
	exit;
fi
touch ${target_file_name}

# collect data
for file in `ls`
do
	if [ ${file}x = ${target_file_name}x ]; then
		continue
	fi
	cat ${file} >> ${target_file_name} &
done

# process
part_size=$[`cat ${target_file_name} | wc -l` / ${parallel_number}]
part_file_name='part_'
for (( i=1; i<=${parallel_number}; i++))
do
	if [ $i -ne ${parallel_number} ]; then
		head -n $[i*${part_size}] ${target_file_name} | tail -n ${part_size} | `python ${base_dir}/phash.py` > ${output_path}${part_file_name}${i} &
	else
		tail -n $[`cat ${target_file_name} | wc -l` - ($i-1)*${part_size}] ${target_file_name} | `python ${base_dir}/${script}` > ${output_path}${part_file_name}${i} &
	fi
done
