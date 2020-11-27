#include "BeControlledState.h"
#include "StateManager.h"

BeControlledState::BeControlledState(Receiver* rec, StateManager* man, Socket* slave) : State(rec, man, NULL){
	_masterSoc = slave;
}

void BeControlledState::startState(){
	int height, width;
	GetDesktopResolution(width, height);
	Message mes(RESOLUTION);

	mes += width;
	mes += height;

	Sleep(1000);
	_masterSoc->sendMessage(&mes);
}

void BeControlledState::handleNextMessage(){
	Message* mes = _masterListen->popMessage();
	Message* answer = NULL;
	COORD pos;
	std::string key = "a";

	if (mes == NULL){
		Sleep(100);
		return;
	}

	switch (mes->getCommand())
	{
	case P2P_SEND_MOUSE:
		if (mes->numOfParametes() > 2){
			std::cout << "x : " << (*mes)[1] << "  y : " << (*mes)[2] << "  click : " << (*mes)[3] << std::endl;
			pos.X = std::stoi((*mes)[1]);
			pos.Y = std::stoi((*mes)[2]);
			moveMouse(pos, std::stoi((*mes)[2]));
		}
		else if (mes->numOfParametes() > 1){
			std::cout << "x : " << (*mes)[1] << "  y : " << (*mes)[2] << std::endl;
			pos.X = std::stoi((*mes)[1]);
			pos.Y = std::stoi((*mes)[2]);
			moveMouse(pos, 0);
		}
		else {
			std::cout << "click : " << (*mes)[1] << std::endl;
			pos.X = -1;
			moveMouse(pos, std::stoi((*mes)[1]));
		}
		break;
	case P2P_SEND_KEYBOARD:
		key[0] = (char)std::stoi((*mes)[1]);
		setKeyBoard(key);
		break;
	default:
		std::cout << "Control state - Dropping message.  Command : " << (*mes)[1] << std::endl;
		break;
	}

	delete mes;
	delete answer;
}

void BeControlledState::setMasterSoc(Socket* soc){
	_masterSoc = soc;
}

void BeControlledState::setReceiver(Receiver* rec){
	_masterListen = rec;
}

void BeControlledState::GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

void BeControlledState::moveMouse(COORD p, int num){

	INPUT    Input = { 0 };
	Input.type = INPUT_MOUSE;

	SetCursorPos(p.X, p.Y);

	switch (num)
	{
	case 1:
		Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;								// We are setting left mouse button down and up.
		SendInput(1, &Input, sizeof(INPUT));

		Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;								// We are setting left mouse button down and up.
		SendInput(1, &Input, sizeof(INPUT));
		break;
	case 2:
		Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;								// We are setting right mouse down and up.
		SendInput(1, &Input, sizeof(INPUT));

		Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;								// We are setting right mouse down and up.
		SendInput(1, &Input, sizeof(INPUT));
		break;
	case 3:
		Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;								// We are setting left mouse button down and up.
		SendInput(1, &Input, sizeof(INPUT));
	case 4:
		Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;								// We are setting right mouse down and up.
		SendInput(1, &Input, sizeof(INPUT));

	default:
		break;
	}

}

void BeControlledState::setKeyBoard(std::string s){
	//ULONG_PTR a = strtoul(s.c_str(), NULL, 0);
	INPUT    Input = { 0 };


	for (int i = 0; i < s.size(); i++){
		Input = { 0 };
		Input.type = INPUT_KEYBOARD;
		Input.ki.wVk = VkKeyScanA(s.at(i));
		SendInput(1, &Input, sizeof(INPUT));
		Input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &Input, sizeof(INPUT));
	}

}

BeControlledState::~BeControlledState(){
}
