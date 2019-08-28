#ifndef SPIDER_H
#define SPIDER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>

/*
Headers da Classe Spider
*/
class Spider{

public:

	Spider(std::string);
	bool valid;
	std::string root; 
	std::string host; 
	

	std::set<std::string> visited_urls; 
	std::map<std::string, std::string> htmls; 
	std::map<std::string, std::string> dictionary; 
	std::map<std::string, std::string> aliases; 
	std::map<std::string, std::set<std::string>> par_child; 

	bool isValid(std::string url);
	std::string parseUrl(std::string url);
	std::string url2filename(std::string url);
	void crawl(int);
	void printCrawled(int);
	void dump(int);

};


#endif