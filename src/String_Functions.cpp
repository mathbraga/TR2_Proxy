
#include "../lib/String_Functions.hpp"

using namespace std;


std::vector<std::string> String_Functions::split(std::string str, const char * delimiter){

    vector<string> tokens;
    string os(delimiter);
    int offset = os.length();

    std::size_t last_found = -1*offset;
    std::size_t found =str.find(delimiter, last_found+offset);
    while(found<str.length()){
        tokens.push_back(str.substr(last_found+offset, found - last_found -offset));
        last_found = found;
        found = str.find(delimiter, last_found+offset);
    }
    if((last_found+offset)<str.length())
        tokens.push_back(str.substr(last_found+offset, str.length()));

    return tokens;
}

std::vector<std::string> String_Functions::splitOnFirst(std::string str, const char * delimiter){

    vector<string> tokens;
    string os(delimiter);
    int offset = os.length();

    std::size_t found =str.find(delimiter);

    if(found > (str.length()-offset)){
        tokens.push_back(str);
        tokens.push_back(str);
        return tokens;
    }

    tokens.push_back(str.substr(0, found));
    tokens.push_back(str.substr(found+offset, str.length()-offset));

    return tokens;
}


int String_Functions::string2file(std::string str, const char *path, const char *file){
    FILE *f;
    string full_path(path);
    full_path.append("/");
    full_path.append(file);
    f = fopen(full_path.c_str(), "wb");
    fwrite( str.data(),str.length(),1, f);
    fclose(f);

    return 0;
}


std::string String_Functions::stringFromFile(const char*file){

    FILE *f;
    char buffer[64768];
    f = fopen(file, "rb");
    fread (buffer, 1, sizeof(buffer), f);
    fclose(f);

    std::string res(buffer);
    return res;
}


std::string String_Functions::replace(std::string file, const char*from,const char*to){

    string str = file;
    string os(from);
    int size = os.length();
    int found = str.find(from);
    while(found<str.length()){
        str.replace(found, size, to);
        found = str.find(from);
    }

    return str;
}
