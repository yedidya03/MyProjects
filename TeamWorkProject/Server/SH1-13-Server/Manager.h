#ifndef _MANAGER_H
#define _MANAGER_H

#include "ClientHandler.h"
#include <thread>

class Manager{
public:
	Manager(std::string DB_URL);
	~Manager();

	void startServer();
	// starts the server action
	

	ClientHandler* isUserConnected(std::string username);
private:
	DataBaseHandler _DB;
	std::vector<ClientHandler*> _clients;

	std::mutex DB_Mutex;

	static void threadRun(ClientHandler* obj);
	// every new client a thread is opend with this function
};



#endif