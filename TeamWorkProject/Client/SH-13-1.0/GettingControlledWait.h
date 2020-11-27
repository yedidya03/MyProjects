#ifndef GETTING_CONTROLLED_H
#define GETTING_CONTROLLED_H

#include "Receiver.h"


class GettingControlledWait{
public:
	GettingControlledWait();
	~GettingControlledWait();

	Receiver* startWaiting(Socket** master, std::mutex* m);
private:
	Receiver* _receiver;
	Socket* _masterPipe;

	bool checkPass(Socket* client);
};

#endif