#ifndef HTTP_REQ_H
#define HTTP_REQ_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>

/* Headers da classe HTTP_Request */

class HTTP_Request{

public:

	HTTP_Request(std::string);
	HTTP_Request();

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