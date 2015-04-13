#include "StdAfx.h"
#include "StreamTcpServer.h"
//#undef UNICODE
//#define WIN32_LEAN_AND_MEAN
#define DEFAULT_PORT 8888
//#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

StreamTcpServer::StreamTcpServer()
{
	PHOSTENT hostinfo;
	WSAData wsaData;
	WORD DLLVSERION;
	DLLVSERION = MAKEWORD(2,1);//Winsocket-DLL 版本
	char *ip;
	//用 WSAStartup 開始 Winsocket-DLL
	int r = WSAStartup(DLLVSERION, &wsaData);

	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		printf("gethostname failed with error: %d\n", WSAGetLastError());
	}else{
		printf("server: %s\n", ac);

		if((hostinfo = gethostbyname(ac)) != NULL)
		{
			int nCount = 0;
			while(hostinfo->h_addr_list[nCount])
			{
				ip = inet_ntoa(*(struct in_addr *)hostinfo->h_addr_list[nCount]);
				printf("IP #%d: %s:%d\n", ++nCount, ip,DEFAULT_PORT);
			}
		}
	}

	//宣告 socket 位址資訊(不同的通訊,有不同的位址資訊,所以會有不同的資料結構存放這些位址資訊)
	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);

	//建立 socket
	SOCKET sListen; //listening for an incoming connection
	SOCKET sConnect; //operating if a connection was found

	//AF_INET：表示建立的 socket 屬於 internet family
	//SOCK_STREAM：表示建立的 socket 是 connection-oriented socket 
	sConnect = socket(AF_INET, SOCK_STREAM, NULL);

	//設定位址資訊的資料
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEFAULT_PORT);

	//設定 Listen
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);//SOMAXCONN: listening without any limit

	//等待連線
	SOCKADDR_IN clinetAddr;

	printf("waiting...\n");

	if(ClientSocket = accept(sListen, (SOCKADDR*)&clinetAddr, &addrlen))
	{
		printf("a connection was found\n");
		printf("server: got connection from %s\n", inet_ntoa(addr.sin_addr));
	}
	closesocket(sListen);
}


StreamTcpServer::~StreamTcpServer()
{
	int iResult;
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return ;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
}

void getBytes(byte* b,  int i) {
	//byte[] b = new byte[4];
	b[0] = (byte) ((i >> 24) & 0xff);
	b[1] = (byte) ((i >> 16) & 0xff);
	b[2] = (byte) ((i >> 8) & 0xff);
	b[3] = (byte) (i & 0xff);
}


void StreamTcpServer::Send(byte data[],int size){
	int iSendResult;
	//傳送訊息給 client 端
	byte len[4]={0x0,0x0,0x0,0x0};
	getBytes(len,size);
	iSendResult = send( ClientSocket, (const char*)len, 4, 0 );
	iSendResult = send( ClientSocket, (const char*)data, size, 0 );
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return;
	}

	printf("Bytes sent: %d\n", iSendResult);
}
