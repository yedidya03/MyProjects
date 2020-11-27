#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>

#define CLOSE_CONNECTION 999

#define SIGN_REQUEST 200
#define SIGN_SUCCEEDED 201
#define SIGN_FAILD 202

#define LOGIN_REQUEST 300
#define LOGIN_SUCCEEDED 301
#define LOGIN_FAILD 302

#define SEARCH_REQUEST 400
#define SEARCH_SUCCEEDED 401
#define SEARCH_FAILD 402

#define CONTROL_USER 500
#define USER_ACCEPTED 501
#define USER_DENIED 502
#define USER_NOT_CONNECTED 503

#define CONTROL_REQUEST 520
#define REQUEST_ACCEPTED 521
#define REQUEST_DENIED 522
#define RAND_PASS 523

#define IP_ADDR_REQUEST 600
#define IP_ADDRESS 601


class Message{
public:
	Message();
	Message(const Message* m);
	Message(int command);
	Message(std::string mes);

	void addParameter(std::string param);
	void addParameter(int param);
	void operator+=(std::string param);
	void operator+=(int param);

	void setCommand(int command);

	std::string operator[](int index); //returns everything as strings (even integers)
	int getCommand(); // returns only the command
	std::string getMessage();

	int numOfParametes();

private:
	int _command;
	std::vector<std::pair<int, std::string>> _parameters;

	std::string getFirstNumberFromMes(std::string s); //returns the number digits in the beginning of the string
	std::string getLegalData(std::string &mes); //returns 1 if legal
};

#endif