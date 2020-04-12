/*
	主模块
*/

#ifndef _MAIN_H_
#define _MAIN_H_

#include "whole.h"
#include "communication.h"
#include "file.h"

void commandReduce(tcpSocket *socket);                                                                                 // 用于解析命令行参数
bool recvToClient(char *ownFilePath, char *clientFilePath, char *socketNumber, tcpSocket *socket);                     // 用于文件下载
bool sendToClient(char *ownFilePath, char *clientFilePath, char *socketNumber, tcpSocket *socket);                     // 用于文件上传
bool cmdExec(char *cmdChar, char *socketNumber, tcpSocket *socket);                                                    // 远程 cmd

#endif