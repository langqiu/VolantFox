#!/usr/bin/python
import os
import sys
import requests
import json
import random
import urllib2
import av

EXTS = 'jpg', 'jpeg', 'JPG', 'JPEG', 'gif', 'GIF', 'png', 'PNG'

def call_micro_service(service_name,param):
    jo=None
    try:
        r = requests.get("http://localhost:8500/v1/health/service/%s?passing=true"%(service_name,))
        servicejson = json.loads(r.text)

        index = int(random.random()*len(servicejson))
        servicejson = servicejson[index]
        serviceurl = "http://"+str(servicejson["Node"]["Address"])+":"+str(servicejson['Service']['Port'])+"%s"%(param,)
        r = requests.get(serviceurl)
        jo = json.loads(r.text)
    except Exception as e:
        print e

    return jo

if __name__ == '__main__':
    service_name = 'cloud-musical-service'
    param = '/?ids=' + sys.argv[1]
    image_list = call_micro_service(service_name, param)
    for image in image_list:
        video_uri = image['videoUri']
        random_name = video_uri.split('/')[-1]
        #video_path = '/mnt2/wei.hu.data/phash_data/' + random_name
        video_path = './' + random_name
        try:
            r = urllib2.Request(video_uri);
            data = urllib2.urlopen(r, None, 1).read()
            f = open(video_path, 'wb')
            f.write(data)
            f.close()
            os.remove(video_path)
        except Exception, e:
            print str(e)
            continue


