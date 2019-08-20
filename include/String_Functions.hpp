#ifndef STR_FUN_H
#define STR_FUN_H

#include <iostream>
#include <string>
#include <vector>
#include <string.h>

class String_Functions{

public:

static std::vector<std::string> split(std::string, const char*);
static std::vector<std::string> split_on_first(std::string, const char*);
static int string_to_file(std::string str, const char *path,const char *file);
static std::string string_from_file(const char*file);
static std::string replace(std::string,const char*,const char*);

};


#endif