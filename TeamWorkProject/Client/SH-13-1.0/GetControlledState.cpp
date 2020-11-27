#include "GetControlledState.h"
#include "StateManager.h";

GetControlledState::GetControlledState(Receiver* rec, StateManager* man, Socket* server, Socket* master) : State(rec, man, server){
	_masterPipe = master;
}

void GetControlledState::startState(){
	std::string userInput;
	Message mes(REQUEST_ACCEPTED);
	_serverPipe->sendMessage(&mes);

	GettingControlledWait listen2mater;
	_masterListener = listen2mater.startWaiting(&_masterPipe, _manager->getQueueMutex());
}

void GetControlledState::handleNextMessage(){
	Message* mes = _rec->popMessage();
	Message* answer;

	if (mes == NULL){
		Sleep(100);
		return;
	}

	switch (mes->getCommand())
	{
	case USER_CONTROL_REQUEST:
		std::cout << "user control request gotten!" << std::endl;
		if ((*mes)[1] == _password){
			answer = new Message(REQUEST_ACCEPTED);
			// send answer to the other user
			_masterPipe->sendMessage(answer);

			_manager->getBeUnderControl()->setMasterSoc(_masterPipe);
			_manager->getBeUnderControl()->setReceiver(_masterListener);

			// hop to beControlled state
			_manager->setCurrentState((State*)_manager->getBeUnderControl());
		}
		else{
			std::cout << "the conform password that has been sent was'nt currect. check if someone is trying to hack your coputer." << std::endl;
			answer = new Message(REQUEST_DENIED);
			_manager->setCurrentState((State*)_manager->getNormalState());
		}

		// send answer
		_masterPipe->sendMessage(answer);

		// going on to next state
		_manager->getCurrentState()->startState();
		break;
	case RAND_PASS:
		std::cout << "got password" << std::endl;
		_password = (*mes)[1];
		break;
	default:
		std::cout << "Get Controlled state - Dropping message.  Command : " << (*mes)[1] << std::endl;
		break;
	}

	delete mes;
}

void GetControlledState::setMaster(Socket* soc){
	_masterPipe = soc;
}

GetControlledState::~GetControlledState(){
}