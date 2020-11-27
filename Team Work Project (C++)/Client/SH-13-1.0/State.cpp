#include "State.h"
#include "StateManager.h"

State::State(Receiver* rec, StateManager* man, Socket* server){
	_rec = rec;
	_manager = man;
	_serverPipe = server;
}

void State::getControlled(Message* mes){
	std::string userInput;

	std::cout << (*mes)[1] << " wants to control your computer. Do you agree ? (y/n) : ";
	do{
		std::getline(std::cin, userInput);
	} while (userInput == "");

	if (userInput == "y"){
		_manager->setCurrentState((State*)_manager->getGetControlledState());
		_manager->getCurrentState()->startState();
	}
	else{
		Message mes(REQUEST_DENIED);
		_serverPipe->sendMessage(&mes);
	}
}