#ifndef _SOCKET_H
#define _SOCKET_H

#include "Message.h"

#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define IP "127.0.0.1"
#define SERVER_PORT "1678"
#define P2P_PORT "2056"

class Socket{
public:
	Socket();
	Socket(SOCKET s);
	~Socket();

	// server functions
	int ourBind();
	int ourListen(int howMeny);
	Socket* ourAccept();

	int sendMessage(Message* messge);
	Message* recvMessage(int buffSize);

	int isOpen();
	void close();

	std::string getIP();
private:
	SOCKET _sock;
	int iResult;
	struct addrinfo *result;
};

#endif