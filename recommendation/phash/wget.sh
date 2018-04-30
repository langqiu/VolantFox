#!/usr/bin/bash
cd `dirname $0`; base_dir=`pwd`

input_file=${base_dir}'/url.txt'
if [ ! -f ${input_file} ]; then
    echo "input file not exist"
    exit;
fi

cd ${base_dir}'/data'
while read line
do
    wget ${line}
done < ${input_file}
