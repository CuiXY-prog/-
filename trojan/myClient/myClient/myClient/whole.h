/*
	总文件头
*/
#define _CRT_SECURE_NO_WARNINGS
#ifndef _WHOLE_H_
#define _WHOLE_H_

#include <Windows.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
using namespace std;

struct myProtocol // 用于客户端和服务器端传输的协议
{
	/*
		type = 1 用于文件的上传
		type = 2 用于文件的下载
		type = 3 表示客户端或服务端是否准备好传输数据
		type = 4 表示命令执行
		...
	*/
	int type = 0;

	/*
		type = 1 && type = 2
	*/
	int fileSize = 0;
	char clientFile[MAX_PATH] = { 0 };        // 用于上传文件

	/*
		type = 4
	*/
	char cmdChar[MAX_PATH] = { 0 };           // 用于命令执行
};

#endif