#include "Socket.h"

Socket::Socket(){
	WSADATA wsaData;

	struct addrinfo hints;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		_sock = NULL;
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, SERVER_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		_sock = NULL;
		return;
	}

	this->_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (this->_sock == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		_sock = NULL;
		return;
	}
}
Socket::Socket(SOCKET s){
	this->_sock = s;
}


int Socket::ourBind(){
	iResult = bind(this->_sock, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		std::cout << "bind failed with error: %d\n" << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(this->_sock);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	return 0;
}
int Socket::ourListen(int howMeny){
	iResult = listen(this->_sock, howMeny);

	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(this->_sock);
		WSACleanup();
		return 1;
	}

	return 0;
}
Socket* Socket::ourAccept(){
	Socket* ClientSocket;

	ClientSocket = new Socket(accept(this->_sock, NULL, NULL));
	if (ClientSocket->_sock == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(this->_sock);
		WSACleanup();
		return NULL;
	}

	return ClientSocket;
}


int Socket::sendMessage(Message* m){

	iResult = send(_sock, m->getMessage().c_str(), m->getMessage().length(), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(_sock);
		WSACleanup();
		return 1;
	}

	return 0;
}
Message* Socket::recvMessage(int buffSize){
	char* buffer = (char*)malloc(buffSize);

	iResult = recv(_sock, buffer, buffSize, 0);
	std::string s(buffer);
	Message* m = new Message(s);

	delete buffer;
	return m;
}


int Socket::isOpen(){
	if (this->_sock == NULL){
		return 0;
	}

	return 1;
}
void Socket::close(){
	char recvbuf[BUFSIZ];

	// shutdown the connection since no more data will be sent
	iResult = shutdown(_sock, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(_sock);
		WSACleanup();
		return;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(_sock, recvbuf, BUFSIZ, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	closesocket(_sock);
	_sock = NULL;
}

std::string Socket::getIP(){
	Message* mes = new Message(IP_ADDR_REQUEST);
	this->sendMessage(mes);
	delete mes;

	mes = this->recvMessage(BUFSIZ);
	if (mes->getCommand() == IP_ADDRESS){
		return (*mes)[1];
	}

	return "";
}


Socket::~Socket(){
	if (_sock != NULL){
		close();
	}
}