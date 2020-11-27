#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "Message.h"
#include <queue>

class MessageQueue{
public:
	static MessageQueue* getInstance();

	void push(Message* mes);
	void pop();
	Message* front();

	bool empty();
private:
	MessageQueue();

	std::queue<Message*> _messages;
	static MessageQueue* _firstInstance;
};


#endif