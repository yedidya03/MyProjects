#include "NormalState.h"
#include "StateManager.h"

NormalState::NormalState(Receiver* rec, StateManager* man, Socket* server) : State(rec, man, server){
	_logedIn = false;
}

void NormalState::startState(){
	_manager->getCurrentState()->handleNextMessage();
}

void NormalState::handleNextMessage(){
	Message* mes = _rec->popMessage();
	Message* answer;
	std::string userInput;

	if (mes == NULL){
		std::cout << "Choose what to do (write the number of command): " << std::endl;
		if (_manager->getNormalState()->isLogedIn() == false){
			std::cout << "	1 - Sing up" << std::endl;
			std::cout << "	2 - Log in" << std::endl;
			std::cout << "	3 - Exit" << std::endl;
		}
		else {
			std::cout << "	1 - connect to user" << std::endl;
			std::cout << "	2 - Log out" << std::endl;
			std::cout << "	3 - refresh" << std::endl;
		}

		std::cout << "Your choice is : ";
		std::getline(std::cin, userInput);

		if (userInput.empty() || userInput.find_first_not_of("0123456789") != std::string::npos){
			std::cout << "the input you entered is nut legal.. please try again" << std::endl;
			delete mes; 
			return;
		}


		switch (stoi(userInput)){
		case 1:
			if (_manager->getNormalState()->isLogedIn() == false){
				_manager->setCurrentState((State*)_manager->getSignState());
			}
			else{
				_manager->setCurrentState((State*)_manager->getSearchState());
			}
			break;
		case 2:
			if (_manager->getNormalState()->isLogedIn() == false){
				_manager->setCurrentState((State*)_manager->getLoginState());
				break; // if he is loged in the chice is'nt good and go to default
			}
			else{
				this->logout();
			}
			break;
		case 3:
			if (_manager->getNormalState()->isLogedIn() == false){
				std::cout << "closing program..." << std::endl;
			}
			else{
				break;
			}

			delete mes;
			mes = new Message(CLOSE_CONNECTION);
			_serverPipe->sendMessage(mes);
			_serverPipe->close();
			_manager->stopProgram();
			return;
			break;
		default:
			std::cout << "Your choice is'nt good. Please try again" << std::endl;
			break;
		}

		_manager->getCurrentState()->startState();
	}
	else // clearing the queue from remaining messages
	{
		switch (mes->getCommand())
		{
		case CONTROL_REQUEST:
			getControlled(mes);
			break;
		case IP_ADDR_REQUEST:
			answer = new Message(IP_ADDRESS);
			*answer += Socket::getMyIP(IP_ADAPTER);
			_serverPipe->sendMessage(answer);
			break;
		default:
			std::cout << "Normal state - Dropping message.  Command : " << (*mes)[1] << std::endl;
			break;
		}
	}

	delete mes;
}

bool NormalState::isLogedIn(){
	return _logedIn;
}

void NormalState::login(){
	_logedIn = true;
}

void NormalState::logout(){
	_logedIn = false;
}

NormalState::~NormalState(){
}