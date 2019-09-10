#ifndef STR_FUN_H
#define STR_FUN_H

#include <iostream>
#include <string>
#include <vector>
#include <string.h>

class String_Functions{

public:

static std::vector<std::string> split(std::string, const char*);
static std::vector<std::string> splitOnFirst(std::string, const char*);
static int string2file(std::string str, const char *path,const char *file);
static std::string stringFromFile(const char*file);
static std::string replace(std::string,const char*,const char*);

};


#endif