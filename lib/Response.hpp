#ifndef Response_Header
#define Response_Header

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>

class Response{

public:

	Response(std::string);
	Response();

	std::string status_code;
	std::string version;
	std::map<std::string, std::string> fields; 
	std::string data;
	void print();
	std::string assembly();


};


#endif