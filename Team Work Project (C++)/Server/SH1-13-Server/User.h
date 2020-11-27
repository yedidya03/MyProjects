#ifndef _USER_H
#define _USER_H

#include "Socket.h"

struct User{
	std::string _username;
	std::string _password;
	std::string _email;
	std::string _question; // question for password forgoten
	std::string _answer; // answer for the question
	std::string _name; // first name
	std::string _family; //last name
	std::string _company;
};


#endif