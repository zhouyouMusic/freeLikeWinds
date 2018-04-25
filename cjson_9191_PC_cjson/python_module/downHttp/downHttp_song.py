#!/usr/bin/python
#coding=utf-8
import os
import urllib
import urllib2

def searchSongInfo(savFile,logFile):
    with open(logFile) as fp:
        linesCont = fp.readlines()
        lineLength = len(linesCont)
        while lineLength > 1:
            lineCt = linesCont[lineLength-1]
            gbkStr = unicode(lineCt,'utf-8')
            lineLength = lineLength - 1 
            if "songJun" in gbkStr:
                strGbk = gbkStr
                utfString = strGbk.encode("utf-8")                
                listUrl = strGbk.split('|')
                fSongName = listUrl[2].split('\"')[1].split('/')[0] + "-" + listUrl[3].split('\"')[1] + ".mp3"          
                reqUrl = listUrl[1].split('\"')[1]
                totalPath = savFile + os.path.sep + fSongName
                fs = open(totalPath,"w")
                reqHand = urllib2.urlopen(reqUrl)
                reqCont = reqHand.read()
                fs.write(reqCont)
                break

#searchSongInfo("../music","../../logSystem/R16_Runing.log")

