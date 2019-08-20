/*
    A classe HTTP_Response tem a função de fazer o parse da resposta HTTP, para posterior análise e tratamento.

*/

#include "../include/HTTP_Response.hpp"
#include "../include/String_Functions.hpp"

using namespace std;
// construtor que faz o parse
HTTP_Response::HTTP_Response(std::string Response_String){
    //spliting (header from body)
    vector<string> splitted = String_Functions::split(Response_String, "\r\n\r\n");
    if(splitted.size()<2)
        splitted = String_Functions::split_on_first(Response_String, "\n\n");
    
    string header = splitted[0];
	data = splitted[1];
 
    //parsing first line
	vector<string> hd_lines = String_Functions::split(header, "\r\n");
	string first_line = hd_lines[0];
	vector<string> fl_tokens = String_Functions::split_on_first(first_line, " ");
    version = fl_tokens[0];
    status_code = fl_tokens[1];
    
    //parsing fields
    vector<string> temp;
    for( int i = 1; i<hd_lines.size(); i++){
         temp = String_Functions::split_on_first(hd_lines[i], " ");
         fields[temp[0]] = temp[1];
    }

};

//construtor vazio
HTTP_Response::HTTP_Response(){
    

};
// método que monta a resposta de volta a uma string para posterior envio
std::string HTTP_Response::assembly(){
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

// método que printa a response (para debug)
void HTTP_Response::print(){
    cout << "status code: " << status_code << endl;
    cout << "version: " << version << endl;

    for(map<string, string>::iterator it = fields.begin(); it!=fields.end(); ++it){
        cout << it->first << " " << it->second << endl;
    }
    cout << "data: " << endl << data <<endl;

    //cout << "Assembled: " << endl << assembly() << endl;
}