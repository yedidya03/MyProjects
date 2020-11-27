#ifndef STATE_H
#define STATE_H

#include "Receiver.h"
#include "GettingControlledWait.h"

class StateManager;

class State{
public:
	State(Receiver* rec, StateManager* man, Socket* server);

	virtual void startState() = 0;
	virtual void handleNextMessage() = 0;

	void getControlled(Message* mes);
protected:
	Receiver* _rec;
	StateManager* _manager;
	Socket* _serverPipe;

};


#endif