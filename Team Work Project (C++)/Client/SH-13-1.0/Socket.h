#ifndef _SOCKET_H
#define _SOCKET_H

#include "Message.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define TIMEOUT 5
#define SERVER_IP "192.168.21.56"
//#define SERVER_IP "127.0.0.1"
#define SERVER_PORT "1678"
#define P2P_PORT "2056"

#define IP_ADAPTER "VMnet1"


class Socket{
public:
	Socket();
	Socket(SOCKET s);
	~Socket();

	// server functions
	int ourBind();
	int ourListen(int howMeny);
	Socket* ourAccept();

	int connectTo(std::string address, std::string port);
	// this function connects to the IP and port given, and opens a socket with the server.
	
	
	int sendMessage(Message* messge);
	// this function gets a message and sends it to the socket.
	Message* recvMessage(int buffSize);
	// this function waits for a message from the socket and returns it in arivele
	// the waiting is limitted to TIMEOUT seconds


	int isOpen();
	// indicates which the socket is open or not
	void close();
	// this function closes the socket

	static std::string getMyIP(std::string adapterName);

private:
	SOCKET _sock;
	int iResult;
	struct addrinfo *result;
};

#endif