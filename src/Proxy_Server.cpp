/*
    Na classe Proxy_Server está implementada toda lógica de manipulação de sockets
necessária para implementação do programa. Métodos que lidam com requisições do browser
e envio de requisições HTTP para servidores de origem.

*/

#include "../include/Proxy_Server.hpp"
#include "../include/HTTP_Request.hpp"

int sockfd, client_sockfd, server_sockfd;
struct sockaddr_in address;
struct sockaddr_in serv_addr;
struct sockaddr cli_addr;
int valread;
int opt;
char buffer[64768];

//Gerando socket para escutar na linha
void Proxy_Server::init(int port){ 
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
        perror("Falha ao criar socket do Servidor Proxy");
        exit(EXIT_FAILURE);
    }

//setando as opções do socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&address,sizeof(address))<0)
    {
        perror("Falha ao fazer o bind");
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, 5) < 0)
    {
        perror("Falha ao fazer o listen");
        exit(EXIT_FAILURE);
    }

};

//Inicia a tentativa de handshaking
std::string Proxy_Server::get_client_request(){
    
    using namespace std;

    char buffer[542768];
    bzero((char *) &buffer, sizeof(buffer));
    if ((client_sockfd = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        perror("Falha ao aceitar conexões");
        exit(EXIT_FAILURE);
    }
    cout << "Conectado!" << endl;
    int num = read(client_sockfd , buffer, sizeof(buffer));
    if(num >0){
        string req(buffer);
        HTTP_Request request = HTTP_Request(req);
        request.treat();
        return request.assembly();
    }

    HTTP_Request request = HTTP_Request();
    request.method = "POST";
    return request.assembly();


}

//Comunicação partindo do proxy para o servidor
std::string Proxy_Server::make_request(std::string req){

    using namespace std;

    struct hostent *req_host;
    struct sockaddr_in serv_addr;
    string request = req;

    if((server_sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0);

    HTTP_Request reqst = HTTP_Request(request);
    string host = reqst.fields["Host:"];

    req_host = gethostbyname(host.c_str());
    if ( (req_host == NULL) || (req_host->h_addr == NULL) ) {
        std::cout << "Erro de DNS! " <<  host << std::endl;
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);

    memcpy(&serv_addr.sin_addr.s_addr,req_host->h_addr,sizeof(req_host->h_addr));

    if (connect(server_sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0);
    
    send(server_sockfd, request.c_str(), request.length(), 0);
    
    char buff[1];
    valread = read(server_sockfd, &buff, sizeof(buff));
    string reply(buff); 
    valread = read(server_sockfd, &buff, sizeof(buff));  
    while(valread>0){
        reply.append(buff);
        valread = read(server_sockfd, &buff, sizeof(buff));
    }

    return reply;
}

//Resposta para o browser
void Proxy_Server::reply_client(std::string reply){
    if(send(client_sockfd, reply.c_str(), reply.length(), 0)<0){
        perror("Não foi possível emitir reposta.");
        exit(EXIT_FAILURE);
    }
    close(client_sockfd);
}