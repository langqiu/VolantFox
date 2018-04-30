#!/usr/bin/python

from PIL import Image
import imagehash
import sys
import os

def get_hash_list():
    hash_list = []
    path = sys.argv[1]
    file_path = os.listdir(path)
    i = 0
    index = 0
    for file in file_path:
        if sys.argv[2] == file:
            index = i
        i += 1
        image_file = os.path.join('%s' % file)
        if sys.argv[3] == 'p':
            hash = imagehash.phash(Image.open(path+image_file))
        elif sys.argv[3] == 'd':
            hash = imagehash.dhash(Image.open(path+image_file))
        else:
            hash = imagehash.whash(Image.open(path+image_file))
        hash_list.append(int(str(hash), 16))
        #print int(str(hash), 16)
        print str(int(str(hash), 16))
    return hash_list, index

def hamming(h1, h2):
    h, d = 0, h1 ^ h2
    while d:
        h += 1
        d &= d - 1
    return h

if __name__=='__main__':
    hash_list, index = get_hash_list()
    #print map(lambda j : hamming(hash_list[index], j), hash_list)
    for i in hash_list:
        print map(lambda j : hamming(i, j), hash_list)
