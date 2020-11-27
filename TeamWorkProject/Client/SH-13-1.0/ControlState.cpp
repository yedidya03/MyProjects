#include "ControlState.h"
#include "StateManager.h";

ControlState::ControlState(Receiver* rec, StateManager* man, Socket* slave) : State(rec, man, NULL){

	_sleveSoc = slave;
	_stopSending = false;
	
}

void ControlState::startState(){
	Message* resolutionMes = NULL;
	/*Message request(RESOLUTION_REQUEST);
	_sleveSoc->sendMessage(&request);*/

	while (!(resolutionMes != NULL && resolutionMes->getCommand() == RESOLUTION)){
		resolutionMes = this->_rec->popMessage();
		Sleep(100);
	}
	std::cout << "received!!" << std::endl;
	_sendingDataThread = new std::thread(sendData, stoi((*resolutionMes)[1]), stoi((*resolutionMes)[2]), _sleveSoc, &_stopSending);
}

void ControlState::handleNextMessage(){
	Message* mes = _rec->popMessage();

	if (mes == NULL){
		Sleep(100);
		return;
	}

	switch (mes->getCommand())
	{
	case CLOSE_CONNECTION:
		_stopSending = true;
		_sendingDataThread->join();
		_sleveSoc->close();

		// I'm not shure about those two lines
		_manager->setCurrentState((State*)_manager->getNormalState());
		_manager->getCurrentState()->startState();

		break;
	default:
		std::cout << "Control state - Dropping message.  Command : " << (*mes)[1] << std::endl;
		break;
	}

	delete mes;
}

void ControlState::setSlaveSoc(Socket* soc){
	_sleveSoc = soc;
}

void ControlState::setReceiver(Receiver* rec){
	this->_rec = rec;
}

void ControlState::sendData(int slaveScreenWidth, int slaveScreenHeight, Socket* slave, bool* getOut){
	Message* mouse, *keyboard;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int width, height;

	DWORD NumRead;

	INPUT_RECORD InRec;
	INPUT_RECORD InRec2;
	HANDLE hIn;
	HANDLE hOut;
	POINT p;
	POINT x;

	std::cout << "this is my console" << std::endl;

	AllocConsole();
	HWND myWindow = FindWindowA("ConsoleWindowClass", NULL);

	GetCursorPos(&p);
	ScreenToClient(myWindow, &p);


	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);



	FlushConsoleInputBuffer(hIn);

	while (!(*getOut))
	{
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		width = csbi.dwSize.X * 8;
		height = csbi.dwSize.Y;

		mouse = new Message(P2P_SEND_MOUSE);
		keyboard = new Message(P2P_SEND_KEYBOARD);

		ReadConsoleInput(hIn, &InRec, 128, &NumRead);

		x = p;
		GetCursorPos(&p);
		ScreenToClient(myWindow, &p);

		if (p.x != x.x || p.y != x.y){
			std::cout << "x - " << p.x << "   y - " << p.y << std::endl;
			mouse->addParameter((int)((double)p.x * ((double)slaveScreenWidth / (double)width)));
			mouse->addParameter((int)((double)p.y * ((double)slaveScreenHeight / (double)height)));
			slave->sendMessage(mouse);
			Sleep(40);
		}
		if (InRec.EventType == MOUSEEVENTF_RIGHTDOWN || InRec.EventType == MOUSEEVENTF_LEFTDOWN/* || InRec.EventType == MOUSEEVENTF_LEFTUP || InRec.EventType == MOUSEEVENTF_RIGHTUP*/){
			if (InRec.Event.MouseEvent.dwButtonState != 0){
				std::cout << InRec.Event.MouseEvent.dwButtonState << std::endl;
			}
			mouse->addParameter((int)InRec.Event.MouseEvent.dwButtonState);
		}
		if (InRec.EventType == KEY_EVENT && InRec.Event.KeyEvent.bKeyDown){
			if (InRec.Event.KeyEvent.uChar.AsciiChar >= 0 && InRec.Event.KeyEvent.uChar.AsciiChar <= 255){
				char c = InRec.Event.KeyEvent.uChar.AsciiChar;
				std::cout << "keyboard  : " << c << std::endl;
				keyboard->addParameter(c);
				slave->sendMessage(keyboard);
			}
		}

		delete mouse;
		delete keyboard;
	}
}

ControlState::~ControlState(){
}