#ifndef GET_CONTROLLED_STATE_H
#define GET_CONTROLLED_STATE_H

#include "State.h"

class GetControlledState : State{
public:
	GetControlledState(Receiver* rec, StateManager* man, Socket* server, Socket* master);
	~GetControlledState();

	virtual void startState();
	virtual void handleNextMessage();

	void setMaster(Socket* soc);
private:
	std::string _password;

	Socket* _masterPipe;
	Receiver* _masterListener;
};

#endif