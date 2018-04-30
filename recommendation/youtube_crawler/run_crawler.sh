#!/usr/bin/env bash

python youtube_crawler.py -q 'fight caught on camera,school fight,school body slam,teen body slam,boy slammed,kid slammed,girl slammed,street fight,street ko,street knockout,street brutal fight,street bare knuckle,school ko,school knockout,school brutal fight,school nasty fight,nasty girl fight,nasty high school fights' -p 500 -t 20 -v 100000 -s './fight_video/' -m 20 -u './video_urls' > log

exit
if [[ $? -ne 0 ]]; then
	echo "error"
	exit
fi

cd ./fight_video/
for f in ./*
do
	aws s3 cp "${f}" "s3://musically-bigdata/recommend_script/data/wei_hu_copy/youtube_videos/"
done
