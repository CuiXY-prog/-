/*
	Ö÷Ä£¿é
*/
#ifndef _MAIN_H_
#define _MAIN_H_

#include "whole.h"
#include "file.h"
#include "communication.h"

bool recvToServer(myProtocol *recvInfo, tcpSocket *mysocket);
bool sendToServer(myProtocol *recvInfo, tcpSocket *mysocket);
void protocolParsing(tcpSocket *mysocket);
bool connectServer(tcpSocket *mysocket);
bool systemCmd(myProtocol *recvInfo, tcpSocket *mysocket);

#endif
