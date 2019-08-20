/*
    A classe HTTP_Request tem a função de fazer o parse da requisição HTTP, para posterior análise e tratamento.

*/


#include "../include/HTTP_Request.hpp"
#include "../include/String_Functions.hpp"

using namespace std;

//construtor que faz o parse da request
HTTP_Request::HTTP_Request(std::string Request_String){

    //spliting (header from body)
    vector<string> splitted = String_Functions::split(Request_String, "\r\n\r\n");
    string header = splitted[0];
    if(splitted.size()>1)
        body = splitted[1];

    //splitting in lines
    vector<string> lines = String_Functions::split(header, "\r\n");

    //parsing first line
    string first_line = lines[0];
    vector<string> fl_tokens = String_Functions::split(first_line, " ");
    method = fl_tokens[0];
    url = fl_tokens[1];
    version = fl_tokens[2];

    //parsing fields
    vector<string> temp;
    for( int i = 1; i<lines.size(); i++){
         temp = String_Functions::split_on_first(lines[i], " ");
         fields[temp[0]] = temp[1];
    }

    //lacks the body parser (doesnt accept POST yet)

};

//construtor vazio
HTTP_Request::HTTP_Request(){

    method = "GET";
    version = "HTTP/1.1";
    url = "";
    body = "";

};

// método que monta a request devolta em uma string (pra ser enviada)
std::string HTTP_Request::assembly(){
    using namespace std;
    string Request("");

    Request.append(method);
    Request.append(" ");
    Request.append(url);
    Request.append(" ");
    Request.append(version);
    Request.append("\r\n");

    for (std::map<string,string>::iterator it=fields.begin(); it!=fields.end(); ++it){
        Request.append(it->first);
        Request.append(" ");
        Request.append(it->second);
        Request.append("\r\n");
    }
    Request.append("\r\n");
    Request.append(body);

    return Request;

}

// método que printa a request (para debug)
void HTTP_Request::print(){
    cout << "method: " << method << endl;
    cout << "version: " << version << endl;
    cout << "url: " << url << endl;

    for(map<string, string>::iterator it = fields.begin(); it!=fields.end(); ++it){
        cout << it->first << " " << it->second << endl;
    }
    cout << "body: " << body << endl<<endl;
    //cout << "Assembled: " << endl << assembly() << endl;
}

// método que trata a requisição para melhor funcionamento do programa de acordo com as limitações da implementação
void HTTP_Request::treat(){
    fields["Connection:"] = "close";
    fields["Accept-Encoding:"] = "identity";
}

// método que analisa a requisição, aqui, uma limitação considerável: só aceita método GET
bool HTTP_Request::eval(){
    if( method != "GET") return false;
    return true;
}