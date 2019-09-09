
#include "../lib/Proxy_Server.hpp"
#include "../lib/Request.hpp"

int server_sockfd, client_sockfd, outbound_socket;
struct sockaddr_in address;
struct sockaddr_in serv_addr;
struct sockaddr cli_addr;
int addrlen, valread;
int opt;
char buffer[64768];


void Proxy_Server::init(int port){
    
	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Falha ao criar socket do Servidor Proxy");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_sockfd, (struct sockaddr *)&address,sizeof(address))<0)
    {
        perror("Bind Error");
        exit(EXIT_FAILURE);
    }
    if (listen(server_sockfd, 5) < 0)
    {
        perror("Listen Error");
        exit(EXIT_FAILURE);
    }

};


std::string Proxy_Server::get_client_request(){
    
    using namespace std;

    char buffer[542768];
    
    memset((char *) &buffer, '\0', sizeof(buffer));
    if ((client_sockfd = accept(server_sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        perror("Falha ao aceitar conexões");
        exit(EXIT_FAILURE);
    }
    cout << "Accepted connection!" << endl;
    int num = read(client_sockfd , buffer, sizeof(buffer));
    if(num >0){
        string req(buffer);
        Request request = Request(req);
        request.treat();
        return request.assembly();
    }

    Request request = Request();
    request.method = "POST";
    return request.assembly();


}

std::string Proxy_Server::make_request(std::string req){

    using namespace std;

    struct hostent *req_host;
    struct sockaddr_in serv_addr;
    string request = req;

    if((outbound_socket = socket(AF_INET,SOCK_STREAM,0)) < 0);

    Request reqst = Request(request);
    string host = reqst.fields["Host:"];

    req_host = gethostbyname(host.c_str());
    if ( (req_host == NULL) || (req_host->h_addr == NULL) ) {
        std::cout << "Erro no DNS lookup: " <<  host << std::endl;
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);

    memcpy(&serv_addr.sin_addr.s_addr,req_host->h_addr,sizeof(req_host->h_addr));

    if (connect(outbound_socket,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0);
    
    send(outbound_socket, request.c_str(), request.length(), 0);
    
    char buff[1];
    valread = read(outbound_socket, &buff, sizeof(buff));
    string reply(buff); 
    valread = read(outbound_socket, &buff, sizeof(buff));  
    while(valread>0){
        reply.append(buff);
        valread = read(outbound_socket, &buff, sizeof(buff));
    }

    return reply;


}

void Proxy_Server::reply_client(std::string reply){
    if(send(client_sockfd, reply.c_str(), reply.length(), 0)<0){
        perror("Falha ao responder browser");
        exit(EXIT_FAILURE);
    }
    close(client_sockfd);
}

void Proxy_Server::done(){
    close(server_sockfd);
    close(client_sockfd);
}