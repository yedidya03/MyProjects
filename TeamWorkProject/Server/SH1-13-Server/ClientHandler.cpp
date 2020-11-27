#include "ClientHandler.h"
#include "Manager.h"

ClientHandler::ClientHandler(Socket* clientSocket, DataBaseHandler* DB, Manager* manager, std::string ip){
	_manager = manager;
	_clientSocket = clientSocket;
	_clientUser = new User;
	_DB = DB;
	_userAccept = DID_NOT_ANSWER;
	_ip = ip;
}

int ClientHandler::handleClient(){
	Message* mes = NULL;
	bool finish = false;

	while (!finish){
		mes = _clientSocket->recvMessage(1024);

		if (mes == NULL){
			Sleep(100);
			continue;
		}

		std::cout << "got message from : " << _clientUser->_username  << " message : "  << mes->getMessage() << std::endl;

		switch (mes->getCommand()){
		case CLOSE_CONNECTION:
			std::cout << "closing connection with " << this->getUser()->_username << std::endl;
			finish = true;
		case SIGN_REQUEST:
			this->signingProcces(mes);
			break;
		case LOGIN_REQUEST:
			this->loginProcces(mes);
			break;
		case SEARCH_REQUEST:
			this->searchProcces(mes);
			break;
		case CONTROL_USER:
			this->connectProcces(mes);
			break;
		case REQUEST_ACCEPTED:
			this->_userAccept = ACCEPTED;
			this->sendPasswords();
			std::cout << "password has been sent to " << _clientUser->_username << std::endl;
			break;
		case REQUEST_DENIED:
			this->_userAccept = DENIED;
			std::cout << "user did'nt accept the connection!" << std::endl;
			break;
		default:
			break;
		}
		delete mes;
	}
	this->_clientSocket->close();
	this->_clientUser = NULL;
	return 0;
}

void ClientHandler::signingProcces(Message* mes){
	//message do'nt match the protocol
	if (mes->numOfParametes() != 8){
		mes = new Message(SIGN_FAILD);
		*mes += "number of parameters does not mach!";
		_clientSocket->sendMessage(mes);
		return;
	}

	// check password legility
	if (!checkPasswordLegility((*mes)[2])){
		mes = new Message(SIGN_FAILD);
		*mes += "password is not legal!";
		_clientSocket->sendMessage(mes);
		return;
	}

	// signing
	switch (_DB->signUser((*mes)[1], (*mes)[2], (*mes)[3], (*mes)[4], (*mes)[5],
		(*mes)[6], (*mes)[7], (*mes)[8])){
	case USERNAME_EXIST:
		mes = new Message(SIGN_FAILD);
		*mes += "this username already exist";
		_clientSocket->sendMessage(mes);
		return;
	case EMAIL_EXIST:
		mes = new Message(SIGN_FAILD);
		*mes += "this email already have an acount";
		_clientSocket->sendMessage(mes);
		return;
	case SQL_ERROR:
		mes = new Message(SIGN_FAILD);
		*mes += "problem in signing";
		_clientSocket->sendMessage(mes);
		return;
	case 0:// good
		break;
	}

	// if it's a valied message
	mes = new Message(SIGN_SUCCEEDED);
	_clientSocket->sendMessage(mes);
}

void ClientHandler::loginProcces(Message* mes){
	_DB->getUser((*mes)[1], (*mes)[2], _clientUser);
	if (_clientUser->_username != ""){ // if the login was done succesfully	
		mes = new Message(LOGIN_SUCCEEDED);
	}
	else
	{
		mes = new Message(LOGIN_FAILD);
	}
	_clientSocket->sendMessage(mes);
}

void ClientHandler::searchProcces(Message* mes){
	if ((*mes)[1] == this->getUser()->_username){
		mes = new Message(SEARCH_FAILD);
		(*mes) += "you can't connect to yourself!";
		_clientSocket->sendMessage(mes);
		return;
	}

	std::vector<User> usersFound = _DB->getUsersList((*mes)[1]);

	if (usersFound.size() > 0){
		mes = new Message(SEARCH_SUCCEEDED);
		for (int i = 0; i < usersFound.size(); i++){
			*mes += usersFound[i]._username;
		}

	}
	else
	{
		mes = new Message(SEARCH_FAILD);
	}

	_clientSocket->sendMessage(mes);
}

void ClientHandler::connectProcces(Message* mes){
	// check if the user is connected
	ClientHandler* slave = _manager->isUserConnected((*mes)[1]);
	if (slave == NULL || slave->getUser()->_username == this->getUser()->_username){
		mes = new Message(USER_NOT_CONNECTED);
		_clientSocket->sendMessage(mes);
		return;
	}

	std::string randPassword = random_string(15), slaveIP;

	// send the controlled side CONTROL_REQUEST message
	mes = new Message(CONTROL_REQUEST);
	(*mes) += _clientUser->_username;
	slave->_clientSocket->sendMessage(mes);
	slave->setPassword(randPassword);
	delete mes;

	std::cout << "waiting for user to accept connection..." << std::endl;

	while (slave->userAcceptState() == DID_NOT_ANSWER){
		std::cout << "User did'nt accept yet" << std::endl;
		Sleep(500);
	}

	if (slave->userAcceptState() == ACCEPTED){
		std::cout << "User accepted the connection" << std::endl;

		Message* answer = new Message(USER_ACCEPTED);
		(*answer) += slave->_ip;
		(*answer) += randPassword;
		_clientSocket->sendMessage(answer);
			
		std::cout << "password and IP sent to " << _clientUser->_username << std::endl;
	}
	else {
		std::cout << "User denied the connection" << std::endl;

		Message* answer = new Message(USER_DENIED);
		_clientSocket->sendMessage(answer);
	}

	slave->_userAccept = DID_NOT_ANSWER;
	
}

std::string ClientHandler::random_string(size_t length)
{
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

void ClientHandler::sendPasswords(){
	// if the controlled side accept the control :
	// make a random password and send it to both the controlling and the cntrolled side

	//send the password to the controlled side
	Message* mes = new Message(RAND_PASS);
	*mes += _randPassword;
	this->_clientSocket->sendMessage(mes);
	delete mes;
}

void ClientHandler::setPassword(std::string pass){
	_randPassword = pass;
}

int ClientHandler::userAcceptState(){
	return _userAccept;
}

/*
	this function checks if the password that the user entered is legal for registry

	the parameters of a legal password are :
		1 - at least 8 characters
		2 - at least one capital letter
		3 - at least one number character
*/
bool ClientHandler::checkPasswordLegility(std::string pass){ 
	if (pass.length() < 8){
		return false;
	}

	bool num = false;
	bool capital = false;

	for (int i = 0; i < pass.length(); i++){
		if (pass[i] >= 'A' && pass[i] <= 'Z'){
			capital = true;
		}
		else if (pass[i] >= '0' && pass[i] <= '9'){
			num = true;
		}
	}

	return num && capital;
}

/*
	this function returns true if a user already loged in from this client handler
*/
bool ClientHandler::isLogedIn(){
	if (_clientUser != NULL){
		return true;
	}

	return false;
}

User* ClientHandler::getUser(){
	return _clientUser;
}

ClientHandler::~ClientHandler(){
	delete _clientUser;
	if (_clientSocket){
		_clientSocket->close();
		delete _clientSocket;
	}
}