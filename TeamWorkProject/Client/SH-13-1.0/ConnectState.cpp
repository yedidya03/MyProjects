#include "ConnectState.h"
#include "StateManager.h";

ConnectState::ConnectState(Receiver* rec, StateManager* man, Socket* server) : State(rec, man, server){
}

void ConnectState::startState(){
	std::string userInput;
	Message mes(CONTROL_USER);

	mes += _user2Connect;

	_serverPipe->sendMessage(&mes);

	std::cout << "connecting to '" << _user2Connect << "'. please wait..." << std::endl;
}

void ConnectState::handleNextMessage(){
	Message* mesRecv = _rec->popMessage();
	Message* sendMes;

	if (mesRecv == NULL){
		Sleep(100);
		return;
	}

	switch (mesRecv->getCommand())
	{
	case USER_ACCEPTED:
		std::cout << "got user accepted message" << std::endl;


		sendMes = new Message(USER_CONTROL_REQUEST);
		
		(*sendMes) += (*mesRecv)[2]; // the password
		
		//send to the user the message. ip = (*mesRecv)[1]
		_slavePipe = new Socket();
		_slavePipe->connectTo((*mesRecv)[1], P2P_PORT);

		std::cout << "send : " << sendMes->getMessage() << std::endl;

		_slavePipe->sendMessage(sendMes);

		// activating the listener to the other user
		_slaveListen = new Receiver(_slavePipe, _manager->getQueueMutex());
		_slaveListen->listen2Server();

		
		delete sendMes;
		break;
	case USER_DENIED:
		std::cout << "Error : the user did'nt accept the connection" << std::endl;

		_manager->setCurrentState((State*)_manager->getNormalState());
		_manager->getCurrentState()->startState();
		break;
	case USER_NOT_CONNECTED:
		std::cout << "'" << _user2Connect << "' is not connected. Please try again later." << std::endl;

		_manager->setCurrentState((State*)_manager->getNormalState());
		_manager->getCurrentState()->startState();
		break;
	case REQUEST_ACCEPTED:
		_manager->getControlState()->setReceiver(_rec);
		_manager->getControlState()->setSlaveSoc(_slavePipe);
		_manager->setCurrentState((State*)_manager->getControlState());
		_manager->getCurrentState()->startState();
		break;

	case REQUEST_DENIED:
		std::cout << "Error : problem in connecting, please try again in a few minutes." << std::endl;

		_manager->setCurrentState((State*)_manager->getNormalState());
		_manager->getCurrentState()->startState();
		break;
	case CONTROL_REQUEST:
		getControlled(mesRecv);
		break;
	default:
		std::cout << "Connect state - Dropping message.  Command : " << (*mesRecv).getMessage() << std::endl;
		break;
	}

	delete mesRecv;
}

void ConnectState::setUser2Connect(std::string username){
	_user2Connect = username;
}


ConnectState::~ConnectState(){
}