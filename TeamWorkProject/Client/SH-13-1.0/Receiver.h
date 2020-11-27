#ifndef _RECEIVER_H
#define _RECEIVER_H

#include "Socket.h"
#include <exception>
#include <queue>
#include <thread>
#include <mutex>
#include "MessageQueue.h"

class Receiver{
public:
	Receiver(Socket* pipe, std::mutex* m);
	// a constructor that gets the socket of the server

	Receiver(std::string IP, int port);
	// a constructor thar gets the IP and port to connect ti server and does it by its own

	void listen2Server();
	// this function starts the listenning of massages from the server in a different thread

	void stopListenning();
	// this function stops the listenning and joins the thread

	void lockQueue();
	//this function locks the access to the queue

	Message* popMessage();
	// this function returns the erliest message that arived and has'nt been poped

private:
	MessageQueue* _messages;
	//std::mutex Receiver::_writingLock;
	std::mutex* _queueLock;
	Socket* _serverPipe;

	std::thread* _t;
	int _threadOut;

	static void listenThread(Receiver* obj);
	// this function is for the thread to be able to run (static function)
};


#endif