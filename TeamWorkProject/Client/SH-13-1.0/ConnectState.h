#ifndef CONNECTSTATE_H
#define CONNECTSTATE_H

#include "State.h"

class ConnectState : State{
public:
	ConnectState(Receiver* rec, StateManager* man, Socket* server);
	~ConnectState();

	virtual void startState();
	virtual void handleNextMessage();

	void setUser2Connect(std::string username);
private:
	std::string _user2Connect;

	Socket* _slavePipe;
	Receiver* _slaveListen;

};

#endif