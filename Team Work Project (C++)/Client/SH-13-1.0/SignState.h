#ifndef SIGNSTATE_H
#define SIGNSTATE_H

#include "State.h"

class SignState : State{
public:
	SignState(Receiver* rec, StateManager* man, Socket* server);
	~SignState();

	virtual void startState();
	virtual void handleNextMessage();
};

#endif