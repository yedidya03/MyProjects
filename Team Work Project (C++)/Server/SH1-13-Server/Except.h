#ifndef _EXCEPT_H
#define _EXCEPT_H
#include <iostream>
#include <vector>

struct Except{
	int _argc;
	std::vector<std::string> _argv;

	Except(int argc,char** std){
		_argc = argc;
		for (int i = 0; i < argc; i++){
			_argv.push_back(std[i]);
		}
	}
	
	inline Except operator=(Except a) {
		_argc = a._argc;
		for (int i = 0; i < a._argc; i++){
			_argv.push_back(a._argv[i]);
		}
		return a;
	}
	
	inline Except operator=(Except* a) {
		_argc = a->_argc;
		for (int i = 0; i < a->_argc; i++){
			_argv.push_back(a->_argv[i]);
		}
		return *a;
	}
};


#endif