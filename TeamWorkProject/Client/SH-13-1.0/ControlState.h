#ifndef CONTROLSTATE_H
#define CONTROLSTATE_H

#include "State.h"

class ControlState : State{
public:
	ControlState(Receiver* rec, StateManager* man, Socket* slave);
	~ControlState();

	virtual void startState();
	virtual void handleNextMessage();

	static void sendData(int slaveScreenWidth, int slaveScreenHeight, Socket* slave, bool* getOut);

	void setSlaveSoc(Socket* soc);
	void setReceiver(Receiver* rec);
private:
	bool _stopSending;
	Socket* _sleveSoc;
	std::thread* _sendingDataThread;
};

#endif