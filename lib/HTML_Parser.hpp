#ifndef HTML_PRS_H
#define HTML_PRS_H

#include <regex>
#include <iostream>
#include <string>
#include <string.h>
#include <list>
#include <set>
/* Headers da classe HTML_Parser */
class HTML_Parser{

public:
	static std::set<std::string> get_hrefs(const char*);
	static std::set<std::string> get_sources(const char*);
	static std::set<std::string> get_imports(const char*);
	static std::string isolate_html(const char*);

};


#endif