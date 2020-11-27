#ifndef _CLIENT_HANDLER_H
#define _CLIENT_HANDLER_H

#include "DataBaseHandler.h"

#define ACCEPTED 1
#define DID_NOT_ANSWER 0
#define DENIED -1

class Manager;

class ClientHandler{
public:
	ClientHandler(Socket* clientSocket, DataBaseHandler* DB, Manager* manager, std::string ip);
	// constructor must get the socket of the client and the database of the program

	~ClientHandler();

	int handleClient();
	// this function stats the procceing of handling the client and taking care of him

	Socket getControlled();
	User* getUser();

	int userAcceptState();

	void setPassword(std::string pass);
	std::string random_string(size_t length);
	
private:
	std::string _randPassword;

	Socket* _clientSocket;
	std::string _ip;
	DataBaseHandler* _DB;
	User* _clientUser;
	Manager* _manager;

	int _userAccept;

	void signingProcces(Message* mes);
	void loginProcces(Message* mes);
	void searchProcces(Message* mes);
	void connectProcces(Message* mes);
	void sendPasswords();

	bool checkPasswordLegility(std::string pass);
	bool isLogedIn();
};

#endif