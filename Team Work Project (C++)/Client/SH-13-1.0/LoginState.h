#ifndef LOGINSTATE_H
#define LOGINSTATE_H

#include "State.h"

class LoginState : State{
public:
	LoginState(Receiver* rec, StateManager* man, Socket* server);
	~LoginState();

	virtual void startState();
	virtual void handleNextMessage();
private:

};

#endif