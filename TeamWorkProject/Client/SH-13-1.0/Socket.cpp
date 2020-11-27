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
	iResult = getaddrinfo(NULL, P2P_PORT, &hints, &result);
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

int Socket::connectTo(std::string address, std::string port){
	if (_sock == NULL || _sock == INVALID_SOCKET){
		return 1;
	}

	struct addrinfo *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo(NULL, (PCSTR)port.c_str(), &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		_sock = NULL;
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		_sock = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (_sock == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to a server.
		sockaddr_in clientService;

		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr(address.c_str());
		clientService.sin_port = htons(std::stoi(port));

		// Connect to server.
		iResult = connect(_sock, (SOCKADDR*)&clientService, sizeof(clientService));
		if (iResult == SOCKET_ERROR) {
			closesocket(_sock);
			_sock = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (_sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	return 0;
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
	WSACleanup();
	_sock = NULL;
}

std::string Socket::getMyIP(std::string adapterName){
	std::string line;
	std::ifstream IPFile;
	int offset;
	char* search0 = "IPv4 Address. . . . . . . . . . . :";      // search pattern

	system("ipconfig > ip.txt");

	IPFile.open("ip.txt");
	if (IPFile.is_open())
	{
		while (!IPFile.eof())
		{
			getline(IPFile, line);
			if ((offset = line.find(adapterName, 0)) != std::string::npos)
			{
				while (!IPFile.eof()){
					getline(IPFile, line);
					if ((offset = line.find(search0, 0)) != std::string::npos)
					{
						//   IPv4 Address. . . . . . . . . . . : 1
						//1234567890123456789012345678901234567890     
						line.erase(0, 39);
						IPFile.close();
						break;
					}
				}
				break;
			}
		}
	}
	return line;
}


Socket::~Socket(){
	close();
}