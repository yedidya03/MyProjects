#include "LoginState.h"
#include "StateManager.h";

LoginState::LoginState(Receiver* rec, StateManager* man, Socket* server) : State(rec, man, server){

}

void LoginState::startState(){
	system("cls");

	std::string userInput;
	Message mes(LOGIN_REQUEST);

	if (_manager->getNormalState()->isLogedIn() == true){
		std::cout << "You are already loged in" << std::endl;
	}
	else{
		std::cout << "username : ";
		do{
			std::getline(std::cin, userInput);
		} while (userInput == "");
		mes += userInput;

		std::cout << "password : ";
		do{
			std::getline(std::cin, userInput);
		} while (userInput == "");
		mes += userInput;

		_serverPipe->sendMessage(&mes);
	}
}

void LoginState::handleNextMessage(){
	Message* mes = _rec->popMessage();

	if (mes == NULL){
		Sleep(100);
		return;
	}

	switch (mes->getCommand())
	{
	case LOGIN_SUCCEEDED:
		std::cout << "You are now loged in!" << std::endl;
		_manager->getNormalState()->login();

		_manager->setCurrentState((State*)_manager->getNormalState());
		_manager->getCurrentState()->startState();

		break;
	case LOGIN_FAILD:
		std::cout << "the login was'nt done succefully - error number " << (*mes)[1] << std::endl;

		_manager->setCurrentState((State*)_manager->getNormalState());
		_manager->getCurrentState()->startState();

		break;
	case CONTROL_REQUEST:
		getControlled(mes);
		break;
	default:
		std::cout << "Login state - Dropping message.  Command : " << (*mes)[1] << std::endl;
		break;
	}

	delete mes;
}

LoginState::~LoginState(){
}