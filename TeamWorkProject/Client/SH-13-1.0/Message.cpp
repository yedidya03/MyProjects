#include "Message.h"

Message::Message(){
	_command = 0;
}
Message::Message(const Message* m){
	this->_command = m->_command;
	this->_parameters = m->_parameters;
}
Message::Message(int command){
	this->_command = command;
}
Message::Message(std::string message){

	std::string param;
	std::string num;

	int times;

	num = getFirstNumberFromMes(message);
	if (num == "" || num.length() != 3){
		_command = 0;
		return; // the message is not legal
	}
	_command = atoi(num.c_str()); //gets the commad number from the message
	message = message.substr(num.length() + 1, message.length() - 1); // removing the command from the message

	num = getFirstNumberFromMes(message);
	if (num == ""){
		_command = 0;
		return; // the message is not legal
	}
	times = atoi(num.c_str()); // gets the number of parameters from the message

	message = message.substr(num.length() + 1, message.length() - 1);

	for (int i = 0; i < times; i++){
		param = getLegalData(message);
		if (param == ""){
			_command = 0;
			break;
		}
		_parameters.push_back(std::make_pair(param.length(), param)); //pushing the parameter to the vector
	}

}


void Message::addParameter(std::string param){
	_parameters.push_back(std::make_pair(param.length(), param));
}
void Message::addParameter(int param){
	std::string temp = std::to_string(param);
	_parameters.push_back(std::make_pair(temp.length(), temp));
}
void Message::operator+=(std::string param){
	this->addParameter(param);
}
void Message::operator+=(int param){
	this->addParameter(param);
}


void Message::setCommand(int command){
	_command = command;
}


std::string Message::operator[](int index){
	if (index == 0){
		return std::to_string(_command);
	}
	else if (index - 1 >= _parameters.size()){
		return "";
	}
	else{
		return _parameters[index - 1].second;
	}
}
int Message::getCommand(){
	return _command;
}
std::string Message::getMessage(){
	//first part - command number
	std::string message = std::to_string(_command);
	message += "|";

	//second part - number of parameters
	message += std::to_string(_parameters.size());
	message += "|";

	//thired part - parameters
	for (std::vector<std::pair<int, std::string>>::iterator it = _parameters.begin(); it != _parameters.end(); it++)
	{
		//parameter size (in bytes)
		message += std::to_string(it->first);
		message += "|";

		message += it->second;
	}

	return message;
}


std::string Message::getFirstNumberFromMes(std::string s){
	std::string num = "";

	for (int i = 0; i < s.length(); i++){
		if (s[i] == '|'){
			break;
		}

		if (s[i] < '0' || s[i] > '9'){
			return "";
		}

		num += s[i];
	}

	return num;
}
std::string Message::getLegalData(std::string &mes){
	std::string numOfBytes, data;

	numOfBytes = getFirstNumberFromMes(mes); // gets length (in bytes) of parameter

	if (numOfBytes == "" || mes[numOfBytes.length()] != '|' || stoi(numOfBytes) == 0){
		return "";
	}

	data = mes.substr(numOfBytes.length() + 1, stoi(numOfBytes)); //gets the parameter acording to the length
	mes = mes.substr(numOfBytes.length() + data.length() + 1, mes.length() - 1);

	return data;
}

int Message::numOfParametes(){
	return _parameters.size();
}