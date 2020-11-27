#include "SearchState.h"
#include "StateManager.h"

SearchState::SearchState(Receiver* rec, StateManager* man, Socket* server) : State(rec, man, server){

}

void SearchState::startState(){
	system("cls");

	std::string userInput;
	Message mes(SEARCH_REQUEST);

	std::cout << "Enter a search pattern :" << std::endl;
	std::getline(std::cin, userInput);

	mes += userInput;

	_serverPipe->sendMessage(&mes);
}

void SearchState::handleNextMessage(){
	Message* mes = _rec->popMessage();
	std::string userInput, username;
	bool notCurrect = true;

	if (mes == NULL){
		Sleep(100);
		return;
	}

	switch (mes->getCommand())
	{
	case SEARCH_SUCCEEDED:
		do {
			std::cout << "choose the user to connect to (type cencel to cencel): " << std::endl;
			// need to prvide wrong username
			for (int i = 1; i <= mes->numOfParametes(); i++){
				std::cout << (*mes)[i] << std::endl;
			}

			do{
				std::getline(std::cin, userInput);
			} while (userInput == "");

			for (int i = 1; i <= mes->numOfParametes(); i++){
				if (userInput == (*mes)[i]){
					notCurrect = false;
					username = (*mes)[i];
					break;
				}
			}
		} while (notCurrect);

		// hop to connect state and pass the username
		_manager->getConnectState()->setUser2Connect(username);
		_manager->setCurrentState((State*)_manager->getConnectState());
		_manager->getCurrentState()->startState();

		break;
	case SEARCH_FAILD:
		std::cout << "Search error : " << (*mes)[1] << std::endl;
		
		_manager->setCurrentState((State*)_manager->getNormalState());
		_manager->getCurrentState()->startState();
		break;
	case CONTROL_REQUEST:
		getControlled(mes);
		break;
	default:
		std::cout << "Search state - Dropping message.  Command : " << (*mes)[1] << std::endl;
		break;
	}

	delete mes;
}

SearchState::~SearchState(){
}