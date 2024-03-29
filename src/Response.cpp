
#include "../lib/Response.hpp"
#include "../lib/String_Functions.hpp"

using namespace std;

Response::Response(std::string Response_String){

    vector<string> splitted = String_Functions::split(Response_String, "\r\n\r\n");
    if(splitted.size()<2)
        splitted = String_Functions::splitOnFirst(Response_String, "\n\n");
    
    string header = splitted[0];
	data = splitted[1];
 

	vector<string> hd_lines = String_Functions::split(header, "\r\n");
	string first_line = hd_lines[0];
	vector<string> fl_tokens = String_Functions::splitOnFirst(first_line, " ");
    version = fl_tokens[0];
    status_code = fl_tokens[1];
    
    vector<string> temp;
    for( int i = 1; i<hd_lines.size(); i++){
         temp = String_Functions::splitOnFirst(hd_lines[i], " ");
         fields[temp[0]] = temp[1];
    }

};


Response::Response(){
    
};

std::string Response::assembly(){
    using namespace std;
    string Response("");

    Response.append(version);
    Response.append(" ");
    Response.append(status_code);
    Response.append("\r\n");

    fields["Content-Length:"] = to_string(data.length());

    for (std::map<string,string>::iterator it=fields.begin(); it!=fields.end(); ++it){
        Response.append(it->first);
        Response.append(" ");
        Response.append(it->second);
        Response.append("\r\n");
    }


    Response.append("\r\n");
    Response.append(data);
    Response.append("\r\n");

    return Response;

}


void Response::print(){
    cout << "status code: " << status_code << endl;
    cout << "version: " << version << endl;

    for(map<string, string>::iterator it = fields.begin(); it!=fields.end(); ++it){
        cout << it->first << " " << it->second << endl;
    }
    cout << "data: " << endl << data <<endl;

}