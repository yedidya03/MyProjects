#ifndef _DB_HADLER_H
#define _DB_HANDLER_H

#include "Except.h"
#include "User.h"
#include <vector>
#include <mutex>
#include "sqlite3.h"

#define USERNAME_EXIST 1
#define EMAIL_EXIST 2
#define SQL_ERROR 3

/*
Mutex need to be used at all functions that call the database
*/
class DataBaseHandler{
public:
	DataBaseHandler(std::string url, std::mutex* DB_Mutex);
	~DataBaseHandler();

	void getUser(std::string username, std::string password, User* user);//Tell you if the username and password are correct
	int signUser(std::string username, std::string password, std::string email, std::string question,
		std::string answer, std::string name, std::string family, std::string company);//signs a new user

	std::vector<User> getUsersList(std::string pattern);
	static int callback(void *data, int argc, char **argv, char **azColName);
private:
	std::string _url;
	sqlite3 *db;
	std::mutex* _DB_Mutex;

};


#endif