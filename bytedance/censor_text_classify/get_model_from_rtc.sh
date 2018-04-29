#!/usr/bin/env bash
cd `dirname $0`; base_path=`pwd`

if [[ $# < 2 ]]; then
  echo "usage : $0 model_name model_save_path"
  exit
fi

model_path="${base_path}/$2"
rm -rf ${model_path}
mkdir -p ${model_path}
model_name=$1
#echo "./${model_path}/${model_name}.zip"

curl -X GET "http://10.6.133.175:2107/pipe/model/download_model" -F business=en -F model_name=${model_name} --output ${model_path}/${model_name}.zip

if [[ $? -ne 0 ]]; then
  echo "download model error!"
  exit
fi

cd ${model_path}
unzip ${model_name}.zip
cp cnn-0049.params.cpu params.cpu
