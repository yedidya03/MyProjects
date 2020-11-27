#ifndef _Sql_H
#define _Sql_H

#include "sqlite3.h"
#include <iostream>


/*
Mutex need to be used at all functions that call the database
*/
class Sql{
public:
	
private:
	std::string insert = "INSERT INTO Users (username,password,name,family,email,question,answer,company)" "VALUES ('";
};


#endif