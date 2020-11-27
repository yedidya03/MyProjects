#include "StateManager.h"

StateManager::StateManager(Receiver* rec, Socket* server, std::mutex* m){
	_normal = new NormalState(rec, this, server);
	_sign = new SignState(rec, this, server);
	_login = new LoginState(rec, this, server);
	_search = new SearchState(rec, this, server);
	_connect = new ConnectState(rec, this, server);
	_getControlled = new GetControlledState(rec, this, server, NULL);
	_control = new ControlState(rec, this, server);
	_BeUnderControl = new BeControlledState(NULL, this, NULL);

	_currentState = (State*)_normal;

	this->_keepGoing = true;

	_queueMutex = m;
}

void StateManager::startClient(){
	_currentState->startState();

	while (this->_keepGoing){
		_currentState->handleNextMessage();
	}

}

void StateManager::stopProgram(){
	this->_keepGoing = false;
}

void StateManager::setCurrentState(State* state){
	_currentState = state;
}
State* StateManager::getCurrentState(){
	return _currentState;
}

NormalState* StateManager::getNormalState(){
	return _normal;
}
SignState* StateManager::getSignState(){
	return _sign;
}
LoginState* StateManager::getLoginState(){
	return _login;
}
SearchState* StateManager::getSearchState(){
	return _search;
}
ConnectState* StateManager::getConnectState(){
	return _connect;
}
GetControlledState* StateManager::getGetControlledState(){
	return _getControlled;
}
ControlState* StateManager::getControlState(){
	return _control;
}
BeControlledState* StateManager::getBeUnderControl(){
	return _BeUnderControl;
}


std::mutex* StateManager::getQueueMutex(){
	return _queueMutex;
}

StateManager::~StateManager(){
	delete _normal;
	delete _sign;
	delete _login;
	delete _search;
	delete _connect;
	delete _getControlled;
	delete _control;
}