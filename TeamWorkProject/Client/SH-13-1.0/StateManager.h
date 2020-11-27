#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "LoginState.h"
#include "SearchState.h"
#include "SignState.h"
#include "NormalState.h"
#include "ConnectState.h"
#include "GetControlledState.h"
#include "ControlState.h"
#include "BeControlledState.h"

class StateManager{
public:
	StateManager(Receiver* rec, Socket* server, std::mutex* m);
	~StateManager();

	void startClient();

	void setCurrentState(State* state);
	State* getCurrentState();

	NormalState* getNormalState();
	SignState* getSignState();
	LoginState* getLoginState();
	SearchState* getSearchState();
	ConnectState* getConnectState();
	GetControlledState* getGetControlledState();
	ControlState* getControlState();
	BeControlledState* getBeUnderControl();

	std::mutex* getQueueMutex();

	void stopProgram();

private:
	State* _currentState;

	NormalState* _normal;
	SignState* _sign;
	LoginState* _login;
	SearchState* _search;
	ConnectState* _connect;
	GetControlledState* _getControlled;
	ControlState* _control;
	BeControlledState* _BeUnderControl;

	std::mutex* _queueMutex;
	bool _keepGoing;
};

#endif