#include "DataBaseHandler.h"
#include <fstream>

DataBaseHandler::DataBaseHandler(std::string url, std::mutex* DB_Mutex) {

	if (!std::fstream(url.c_str())){
		std::cout << "The path is not good or the file npt exict!!" << std::endl;
		this->~DataBaseHandler();
		return;
	}
	_DB_Mutex = DB_Mutex;
	int rc = sqlite3_open(url.c_str(), &db);
	_url = url;

	if (rc)
	{
		std::cout << "Can't open database:", std::string(sqlite3_errmsg(db));
		std::cout << std::endl;
		this->~DataBaseHandler();
	}
	else
	{
		std::cout << "Opened database successfully" << std::endl;
	}

	sqlite3_close(db);
}
DataBaseHandler::~DataBaseHandler()
{
	sqlite3_close(db);
}
int DataBaseHandler::signUser(std::string username, std::string password, std::string email, std::string question,
	std::string answer, std::string name, std::string family, std::string company){
	/*this function sign user to the data base 
	if the function sucseed she return 1 else the function return 0 
	
	*/
	
	char *zErrMsg = 0;
	int rc;
	std::string sql;
	rc = sqlite3_open(_url.c_str(), &db);
	//check if the email and the username not exicting
	sql = "SELECT * FROM Users WHERE username = '";
	sql += username + "';";

	try {
		_DB_Mutex->lock();
		rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		_DB_Mutex->unlock();

	}
	catch (Except ex){//if the callback function run
		std::cout << "The username exict!" << std::endl;
		sqlite3_free(zErrMsg);
		_DB_Mutex->unlock();
		return USERNAME_EXIST;
	}

	sql = "SELECT * FROM Users WHERE email = '";
	sql += email + "';";

	try {
		_DB_Mutex->lock();
		rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		_DB_Mutex->unlock();
	}
	catch (Except ex){
		std::cout << "The email exict!" << std::endl;
		sqlite3_free(zErrMsg);
		_DB_Mutex->unlock();
		return EMAIL_EXIST;
	}


	////IF THE VALUES ARE GOOD WE DO INSERT

	sql = "INSERT INTO Users (username,password,name,family,email,question,answer,company)" "VALUES ('";
	sql += username + "','" + password + "','" + name + "','" + family + "','" + email + "','" + question + "','" + answer + "','" + company + "')" + ";";
	_DB_Mutex->lock();
	rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);
	_DB_Mutex->unlock();

	if (rc != SQLITE_OK)
	{
		std::cout << "SQL error: ", std::string(zErrMsg);
		std::cout << std::endl;
		sqlite3_free(zErrMsg);
		return SQL_ERROR;
	}
	else
	{
		std::cout << "Records created successfully\n" << std::endl;
		return 0;
	}
	
}
void DataBaseHandler::getUser(std::string username, std::string password, User* user){
	Except* ex;
	User* userp = (User*)malloc(sizeof(User));
	char **str;
	int argc;
	char *zErrMsg = 0;
	int rc;
	std::string sql = "SELECT * FROM Users WHERE password = '";
	sql += password + "' AND username = '";
	sql += username + "';";

	rc = sqlite3_open(_url.c_str(), &db);

	if (rc)
	{
		std::cout << "Can't open database:", std::string(sqlite3_errmsg(db));
		std::cout << std::endl;
		this->~DataBaseHandler();
	}
	else
	{
		std::cout << "Opened database successfully" << std::endl;
	}


	try {
			_DB_Mutex->lock();	
		rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		_DB_Mutex->unlock();
		std::cout << "The username or the password is uncorrect" << std::endl;
	}
	catch (Except ex) {
		sqlite3_free(zErrMsg);
		_DB_Mutex->unlock();
		if (!strcmp(ex._argv[0].c_str(), username.c_str())){
			std::cout << "welcome " + username << std::endl;
			user->_username = ex._argv[0];
			user->_password = ex._argv[1];
			user->_name = ex._argv[2];
			user->_family = ex._argv[3];
			user->_email = ex._argv[4];
			user->_question = ex._argv[5];
			user->_answer = ex._argv[6];
			user->_company = ex._argv[7];
			return;
		}
		else{
			std::cout << "The username or the password is uncorrect" << std::endl;
		}
		
	}
	sqlite3_close(db);
	_DB_Mutex->unlock();
	user->_username = "";
}
int DataBaseHandler::callback(void *data, int argc, char **argv, char **azColName){
	Except ex(8, argv);

	throw ex;
	return 0;
}
std::vector<User> DataBaseHandler::getUsersList(std::string pattern){
	Except* ex;
	User user;
	std::vector<User> users;
	users.clear();
	char **str;
	int argc;
	char *zErrMsg = 0;
	int rc;
	std::string sql = "SELECT * FROM Users WHERE username = '";
	sql += pattern + "';";

	rc = sqlite3_open(_url.c_str(), &db);

	if (rc)
	{
		std::cout << "Can't open database:", std::string(sqlite3_errmsg(db));
		std::cout << std::endl;
		this->~DataBaseHandler();
	}
	else
	{
		std::cout << "Opened database successfully" << std::endl;
	}

	try {
		_DB_Mutex->lock();
		rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		_DB_Mutex->unlock();
		std::cout << "The username or the password is uncorrect" << std::endl;
		return users;
	}
	catch (Except ex) {
		sqlite3_free(zErrMsg);
		_DB_Mutex->unlock();
		if (!strcmp(ex._argv[0].c_str(), pattern.c_str())){
			std::cout << "welcome " + pattern << std::endl;
			user._username = ex._argv[0];
			user._password = ex._argv[1];
			user._name = ex._argv[2];
			user._family = ex._argv[3];
			user._email = ex._argv[4];
			user._question = ex._argv[5];
			user._answer = ex._argv[6];
			user._company = ex._argv[7];
			users.push_back(user);
			return users;
		}
		std::cout << "The username or the password is uncorrect" << std::endl;
	}
	sqlite3_close(db);

	return users;
}