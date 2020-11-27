#ifndef BECONTROLLED_H
#define BECONTROLLED_H

#include "State.h"

class BeControlledState : State{
public:
	BeControlledState(Receiver* rec, StateManager* man, Socket* server);
	~BeControlledState();

	virtual void startState();
	virtual void handleNextMessage();

	void setMasterSoc(Socket* soc);
	void setReceiver(Receiver* rec);

	void GetDesktopResolution(int& horizontal, int& vertical);

	void moveMouse(COORD p, int num);
	void setKeyBoard(std::string s);
private:
	std::string _user2Connect;

	Socket* _masterSoc;
	Receiver* _masterListen;

};

#endif