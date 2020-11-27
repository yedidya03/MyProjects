#include "SignState.h"
#include "StateManager.h"

SignState::SignState(Receiver* rec, StateManager* man, Socket* server) : State(rec, man, server){

}

void SignState::startState(){
	system("cls");

	std::string params[8] = { "username", "password", "Email", "first name", "last name",
		"company", "recover question", "recover answer" };

	std::string userInput;
	Message mes(SIGN_REQUEST);

	std::cout << "Enter your details :" << std::endl;
	for (int i = 0; i < 8; i++){
		std::cout << params[i] << " : ";
		
		do{
			std::getline(std::cin, userInput);
		} while (userInput == "");
		mes += userInput;
	}

	_serverPipe->sendMessage(&mes);
}

void SignState::handleNextMessage(){
	Message* mes = _rec->popMessage();

	if (mes == NULL){
		Sleep(100);
		return;
	}

	switch (mes->getCommand())
	{
	case SIGN_SUCCEEDED:
		std::cout << "the signing was done succefully!" << std::endl;

		_manager->setCurrentState((State*)_manager->getNormalState());
		_manager->getCurrentState()->startState();

		break;
	case SIGN_FAILD:
		std::cout << "the signing was'nt done succefully - error number " << (*mes)[1] << std::endl;

		_manager->setCurrentState((State*)_manager->getNormalState());
		_manager->getCurrentState()->startState();

		break;
	case CONTROL_REQUEST:
		getControlled(mes);
		break;
	default:
		std::cout << "Singing state - Dropping message.  Command : " << (*mes)[1] << std::endl;
		break;
	}

	delete mes;
}

SignState::~SignState(){
}