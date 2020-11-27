#include "Manager.h"

Manager::Manager(std::string DB_URL) : _DB(DB_URL, &DB_Mutex){
}

void Manager::startServer(){
	Socket* server = new Socket();
	Socket* client;
	std::string ip;

	/*if (!server->isOpen()){
		return;
	}*/

	if (server->ourBind() == 1){
		return;
	}

	if (server->ourListen(SOMAXCONN) == 1){
		return;
	}

	std::cout << "waiting for clients" << std::endl;

	std::vector<std::thread*> threads;

	while (true){
		client = server->ourAccept();
		std::cout << "client connected" << std::endl;

		ip = client->getIP();

		if (ip != ""){
			_clients.push_back(new ClientHandler(client, &_DB, this, ip));

			threads.push_back(new std::thread(threadRun, _clients[_clients.size() - 1]));
		}
	}	

	for (int i = 0; i < threads.size(); i++){
		threads[i]->join();
	}

	delete server;
}

ClientHandler* Manager::isUserConnected(std::string username){
	for (int i = 0; i < _clients.size(); i++){
		if (_clients[i]->getUser() != NULL && _clients[i]->getUser()->_username == username){
			return _clients[i];
		}
	}
	return NULL;
}

void Manager::threadRun(ClientHandler* obj){
	obj->handleClient();
}

Manager::~Manager(){
	for (int i = 0; i < _clients.size(); i++){
		delete _clients[i];
	}
}