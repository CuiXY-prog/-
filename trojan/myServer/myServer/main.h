/*
	��ģ��
*/

#ifndef _MAIN_H_
#define _MAIN_H_

#include "whole.h"
#include "communication.h"
#include "file.h"

void commandReduce(tcpSocket *socket);                                                                                 // ���ڽ��������в���
bool recvToClient(char *ownFilePath, char *clientFilePath, char *socketNumber, tcpSocket *socket);                     // �����ļ�����
bool sendToClient(char *ownFilePath, char *clientFilePath, char *socketNumber, tcpSocket *socket);                     // �����ļ��ϴ�
bool cmdExec(char *cmdChar, char *socketNumber, tcpSocket *socket);                                                    // Զ�� cmd

#endif