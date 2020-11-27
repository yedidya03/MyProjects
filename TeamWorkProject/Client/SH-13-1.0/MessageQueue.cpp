#include "MessageQueue.h"

MessageQueue* MessageQueue::_firstInstance = NULL;

MessageQueue::MessageQueue(){

}

MessageQueue* MessageQueue::getInstance(){
	if (_firstInstance == NULL){
		_firstInstance = new MessageQueue();
		return _firstInstance;
	}

	return _firstInstance;
}

bool MessageQueue::empty(){
	return _messages.empty();
}

void MessageQueue::push(Message* mes){
	_messages.push(mes);
}

Message* MessageQueue::front(){
	return _messages.front();
}

void MessageQueue::pop(){
	_messages.pop();
}