#include "Receiver.h"

Receiver::Receiver(Socket* socket, std::mutex* m){
	_serverPipe = socket;
	_messages = MessageQueue::getInstance();
	_queueLock = m;
}

Receiver::Receiver(std::string IP, int port){
	_serverPipe = new Socket();
	_serverPipe->connectTo(IP, std::to_string(port));
}


/*Receiver::Receiver(std::string IP, int portNumber){
	_serverSocket = new Socket();
	_serverSocket->connectTo(IP, portNumber);
}*/

Message* Receiver::popMessage(){

	Message* ret = NULL;
	this->_queueLock->lock();
	if (!this->_messages->empty()){
		ret = this->_messages->front();
		this->_messages->pop();
	}
	this->_queueLock->unlock();
	return ret;
}

void Receiver::listen2Server(){
	_t = new std::thread(listenThread, this);
}

void Receiver::listenThread(Receiver* obj){
	obj->_threadOut = 0;
	while (obj->_threadOut != 1){
		Message* mes = obj->_serverPipe->recvMessage(1024);
		std::cout << "got message : " << mes->getMessage() << std::endl;
		if (mes != NULL){
			obj->_queueLock->lock();
			obj->_messages->push(mes);
			obj->_queueLock->unlock();
		}
		else{
			Sleep(100);
		}
	}
}

void Receiver::lockQueue(){
	_queueLock->lock();
}

void Receiver::stopListenning(){
	this->_threadOut = 1;
	_t->join();
}