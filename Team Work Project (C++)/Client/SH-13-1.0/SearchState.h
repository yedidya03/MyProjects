#ifndef SEARCHSTATE_H
#define SEARCHSTATE_H

#include "State.h"

class SearchState : State{
public:
	SearchState(Receiver* rec, StateManager* man, Socket* server);
	~SearchState();

	virtual void startState();
	virtual void handleNextMessage();

private:

};

#endif