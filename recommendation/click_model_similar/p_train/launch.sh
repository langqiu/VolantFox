#!/usr/bin/env python
cd `dirname $0`; base_dir=`pwd`

sh ${base_dir}/train/submit_similar.sh '2017' '11' '21' '20171122' > ${base_dir}/train/train.log 2>&1
sh ${base_dir}/evaluate/submit_evaluate_similar.sh '2017' '11' '21' '20171122' > ${base_dir}/evaluate/evaluate.log 2>&1

