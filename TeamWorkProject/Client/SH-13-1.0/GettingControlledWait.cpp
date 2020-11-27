#include "GettingControlledWait.h"

GettingControlledWait::GettingControlledWait(){
}

Receiver* GettingControlledWait::startWaiting(Socket** master, std::mutex* m){
	Socket* server = new Socket();

	std::string answer;

	if (!server->isOpen()){
		return NULL;
	}

	if (server->ourBind() == 1){
		return NULL;
	}

	if (server->ourListen(SOMAXCONN) == 1){
		return NULL;
	}

	std::cout << "waiting for master" << std::endl;

	// accept the user trying to control this computer
	_masterPipe = server->ourAccept();
	//gcl->_masterSoc = server->ourAccept();
	*master = _masterPipe;

	//*master = server->ourAccept();

	std::cout << "client connected" << std::endl;
	delete _receiver;
	_receiver = new Receiver(_masterPipe, m);
	_receiver->listen2Server();
	return _receiver;
}

bool GettingControlledWait::checkPass(Socket* client){
	return true;
}

GettingControlledWait::~GettingControlledWait(){
}
