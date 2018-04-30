#!/usr/bin/env python
#coding = utf-8

import sys, getopt
import urllib2
import re
from pytube import YouTube
from time import sleep
from Queue import Queue
from threading import Thread
import math

STOP_WORDS = set(['street fighter'])
SEARCH_URL_PREFIX = 'https://www.youtube.com/results?search_query='
PAGE_URL_SUFFIX = '&lclk=short&filters=short&page='
DETAIL_URL_PREFIX = 'https://www.youtube.com/watch?v='
HEADER = {
    'User-Agent' : 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11',
    'Accept' : 'text/html;q=0.9,*/*;q=0.8',
    'Accept-Charset' : 'ISO-8859-1,utf-8;q=0.7,*;q=0.3',
    'Accept-Encoding' : 'utf-8',
    'Connection' : 'close',
    'Referer' : None,
}
TIME_OUT = 10
q = Queue()

def getSearchPageHtml(queryWordList, pageNum):
    if len(queryWordList) == 0:
        return ''
    # get the query url prefix
    url_prefix = SEARCH_URL_PREFIX
    for word in queryWordList:
        url_prefix = url_prefix + word + '+'
    url_prefix = url_prefix[0:-1] + PAGE_URL_SUFFIX
    # get html
    url = url_prefix + str(pageNum)
    request = urllib2.Request(url, None, HEADER)
    response = urllib2.urlopen(request, None, TIME_OUT)
    html = response.read()
    return html

def parseQuery(query):
    # split by space
    query_word_list = query.strip().split(' ')
    return query_word_list

def getSearchPageVideo(html):
    # pattern match and get the video
    pattern = re.compile(r'href="\/watch\?v=([^"]+)"')
    video_list = pattern.findall(html)
    # remove those duplicate videos
    video_list = list(set(video_list))
    return video_list

def getVideoListThread(query,video_list,page_start,page_end):
    # parse query to word list
    query_word_list = parseQuery(query)
    # get the search result in a format of html
    print 'processing \'' + query + '\'......'
    for page_num in range(page_start, page_end+1):
        print '    >>>> page >>>> ' + query + ' ' + str(page_num)
        html = getSearchPageHtml(query_word_list, page_num)
        # parse html and get the video
        video_list += getSearchPageVideo(html)
        sys.stdout.flush()
    print 'finish \'' + query + '\'.'

def downloadVideoThread():
    while True:
        arguments = q.get()
        downloadVideo(arguments)
        sleep(0.01)
        q.task_done()

def downloadVideo(video):
    global _save_path
    global _f
    url_prefix = DETAIL_URL_PREFIX
    url = url_prefix + video
    try:
        # parse video info
        request = urllib2.Request(url, None, HEADER)
        response = urllib2.urlopen(request, None, TIME_OUT)
        html = response.read()
        # stop word filter
        has_stop_word = False
        for word in STOP_WORDS:
            if word in html.lower():
                has_stop_word = True
                break
        if has_stop_word:
            _f.write(url+'\t'+'stop word\n')
            return
        # duration filter
        duration = _minutes_video+1
        pattern = re.compile(r'<meta itemprop="duration" content="PT([0-9]+)M')
        time_list = pattern.findall(html)
        if len(time_list) != 1:
            _f.write(url+'\t'+'duration error\n')
            return
        duration = int(time_list[0])
        # long video filter
        if duration > _minutes_video:
            _f.write(url+'\t'+'long video\n')
            return
        # download
        YouTube(url).streams.first().download(_save_path)
        _f.write(url+'\t'+'success\n')
    except Exception, e:
        # error
        _f.write(url+'\t'+str(e)+'\n')
        return

def main():
    global _query_list
    global _page_nums
    global _url_file
    global _thread_nums
    thread_list = []
    video_list = []
    job_nums = 10
    page_intervals = 50

    sys.stdout.flush()
    # get query video list in multi thread
    print 'thread size before : ' + str(len(thread_list))
    for query in _query_list.split(','):
        for i in range(1,int(math.floor(_page_nums/page_intervals+1.0000001))):
            page_start = (i-1)*page_intervals+1
            page_end = i*page_intervals
            print 'fuck sum : ' + query + '\t' + str(page_start) + '\t' + str(page_end)
            t = Thread(target=getVideoListThread, args=(query,video_list,page_start,page_end,))
            t.setDaemon(True)
            t.start()
            thread_list.append(t)
    # wait
    for i in thread_list:
        i.join()
    t.join()
    print 'thread size after : ' + str(len(thread_list))

    sys.stdout.flush()
    # duplicate
    print 'video nums before : ' + str(len(video_list))
    video_list = list(set(video_list))
    print 'video nums after : ' + str(len(video_list))

    sys.stdout.flush()
    ## new download thread
    #thread_list = []
    #print 'thread size before: ' + str(len(thread_list))
    #for i in range(_thread_nums):
    #    t = Thread(target=downloadVideoThread)
    #    t.setDaemon(True)
    #    t.start()
    #    thread_list.append(t)
    #print 'thread size after : ' + str(len(thread_list))

    ## download video
    #nums = 0
    #for video in video_list:
    #    q.put(video)
    #    nums += 1
    #    if nums >= _video_nums:
    #        break
    #q.join()

def usage(): 
    print '''
    usage: python youtube_crawler.py [option] [arg] 

    options and args: 
    -q  :   search query, use space to separate words
    -p  :   the maximum of search page, 20 videos in each page
    -t  :   thread nums
    -v  :   the maximum of video to be downloaded
    -s  :   path to save videos
    -m  :   the maximum of video duration, count in minutes
    -u  :   match video urls

    default : python youtube_crawler.py -q 'street fight' -p 100 -t 20 -v 100 -s ./fight_video -m 20 -u ./video_urls
    '''

if __name__ == '__main__':
    _query_list = 'street fight'
    _page_nums = 100
    _thread_nums = 20
    _video_nums = 100
    _save_path = './fight_video'
    _minutes_video = 20
    _url_file = './video_urls'

    opts, args = getopt.getopt(sys.argv[1:], "hq:p:t:v:s:m:u:")
    for op, value in opts:
        if op == '-q':
            _query_list = value
        elif op == '-p':
            _page_nums = int(value)
        elif op == '-t':
            _thread_nums = int(value)
        elif op == '-v':
            _video_nums = int(value)
        elif op == '-s':
            _save_path = value
        elif op == '-m':
            _minutes_video = int(value)
        elif op == '-u':
            _url_file = value
        else:
            usage()
            sys.exit()
    
    _f = open(_url_file, 'w')
    main()
    _f.close()
