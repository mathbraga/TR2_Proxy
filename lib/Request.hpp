#ifndef Request_Header
#define Request_Header

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>

/* Headers da classe HTTP_Request */

class Request{

public:

	Request(std::string);
	Request();

	std::string method;
	std::string url;
	std::string version;
	std::map<std::string, std::string> fields; 
	std::string body;
	void print();
	void treat();
	std::string assembly();

};


#endif