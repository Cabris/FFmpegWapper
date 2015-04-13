#pragma once
//#include <windows.h>
//#include <stdlib.h>
//#include <stdio.h>


class StreamTcpServer
{
public:
	StreamTcpServer();
	~StreamTcpServer();
	void Send(byte[] ,int);
	SOCKET ClientSocket;
};

