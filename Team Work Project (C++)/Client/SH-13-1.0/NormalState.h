#ifndef NORMALSTATE_H
#define NORMALSTATE_H

#include "State.h"
#include <exception>
class NormalState : State{
public:
	NormalState(Receiver* rec, StateManager* man, Socket* server);
	~NormalState();

	virtual void startState();
	virtual void handleNextMessage();

	bool isLogedIn();
	void login();
	void logout();

private:
	bool _logedIn;
};

#endif