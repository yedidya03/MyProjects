#include "Manager.h"

int main(){

	std::string url = "test.db";
	Manager manager(url);

	manager.startServer();
	
	std::system("pause");
	return 0;
}