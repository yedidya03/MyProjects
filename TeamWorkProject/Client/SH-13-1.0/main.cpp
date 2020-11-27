#include "GettingControlledWait.h"
#include "StateManager.h"
#include <mutex>


int main(){
	Socket* s = new Socket();
	std::mutex queueMutex;
	Message* mes;

	s->connectTo(SERVER_IP, SERVER_PORT);

	if (s->isOpen()){

		mes = s->recvMessage(BUFSIZ);
		if (mes->getCommand() == IP_ADDR_REQUEST){
			delete mes;
			mes = new Message(IP_ADDRESS);
			*mes += Socket::getMyIP(IP_ADAPTER);
			s->sendMessage(mes);
			delete mes;

			Receiver receiver(s, &queueMutex);
			receiver.listen2Server();

			StateManager mechine(&receiver, s, &queueMutex);

			mechine.startClient();
			receiver.stopListenning();
		}
		else {
			std::cout << "connection fault, sorry but try again in a few minutes..." << std::endl;
		}
	}
	
	std::system("pause");
	return 0;
}