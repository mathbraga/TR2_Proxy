/*
	Classe principal que implementa a Command Line Interface, que printa menus e define a excução do Aracne

*/

#include <iostream>
#include "../lib/HTML_Parser.hpp"
#include "../lib/Spider.hpp"
#include "../lib/Proxy_Server.hpp"
#include "../lib/Request.hpp"
#include "../lib/Response.hpp"
#include "../lib/String_Functions.hpp"

using namespace std;

int port,test=1;

void cmdInput(int argc, char **argv){
	port = 8228;
	if(argc==2){
		string arg(argv[1]);
		if(arg.find("-p")==0){ 
			arg = arg.substr(2,arg.length());
		 	if(atoi(arg.c_str())>0 && atoi(arg.c_str()) <100000)
				port = atoi(arg.c_str());
			else{
				perror("Invalid Port!");
				exit(EXIT_FAILURE);
			}
		}
	}
	if(argc==3){
		string arg(argv[1]);
		if(arg=="-p")
		{	
			if(atoi(argv[2])>0 && atoi(argv[2])<100000)
				port = atoi(argv[2]);
			else{
				perror("Invalid Port!");
				exit(EXIT_FAILURE);
			}
		}
	}
	cout <<"Listening to Port "<< port << endl;
	std::cin.ignore();
}


void sniffer(){
	Proxy_Server server = Proxy_Server();
	server.init(port);
	string wait;
	char key;

	system("clear");
	cout << "Traffic inspector is listening on port " << port << endl;
	cout << "Waiting for Request..." << endl;	
	system("clear");
	cout << "Traffic inspector is listening on port " << port << endl;
	cout << "Waiting for Request..." << endl;

	string req = server.get_client_request();
	Request request = Request(req);
	cout << "Request received - Host: " << request.fields["Host:"] << " URL: " << request.url << endl;
	String_Functions::string_to_file(req, ".", "request.txt");
	cout << "Edit Request? (Y/N)" << endl;
	cin >> key;
	if(key=='Y' || key=='y'){
		system("notepad.exe request.txt");
	}
	else{
		std::cin.ignore();
	}
	req = String_Functions::string_from_file("request.txt");


	string reply = server.make_request(req);
	Response response = Response(reply);
	String_Functions::string_to_file(reply, ".", "response.txt");
	cout << "Edit Response from " << request.fields["Host:"] << "? (Y/N)" << endl;
	cin >> key;
	if(key=='Y' || key=='y'){
		system("notepad.exe response.txt");
	}
	else{
		std::cin.ignore();
	}
	reply = String_Functions::string_from_file("response.txt");
	server.reply_client(reply);
	server.done();
	cout << "Go back to menu (ENTER)" << endl;
	std::cin.ignore();
	system("clear");
	return;
	
}


void crawler(){
	string url;
	system("clear");
	cout << "Crawler" << endl;
	cout << "URL to crawl: ";
	cin >> url;
	Spider spider = Spider(url);

	while(!spider.valid){
		system("clear");
		cout << "Invalid URL" << endl;
		cout << "URL to crawl: ";
		cin >> url;
		spider = Spider(url);
	}

	spider.crawl(test);
	spider.printCrawled(test);
	cout << "Go back to menu (ENTER)" << endl;
	std::cin.ignore();
	std::cin.ignore();
}


void dump(){
	
	string url;
	system("clear");
	cout << "Dump website" << endl;
	cout << "URL to dump: ";
	cin >> url;
	Spider spider = Spider(url);

	while(!spider.valid){
		system("clear");
		cout << "You have typed an invalid url!" << endl;
		cout << "Dump website" << endl;
		cout << "URL to dump: ";
		cin >> url;
		spider = Spider(url);
	}
	spider.dump(test);
	cout << "Go back to menu (ENTER)" << endl;
	std::cin.ignore();
	std::cin.ignore();

}

int main(int argc, char **argv){

	int key;

	cmdInput(argc, argv);
	while(1){
		system("clear");
		cout << "Select an Option: " << endl;
		cout << "1 - Sniff" << endl;
		cout << "2 - Spider" << endl;
		cout << "3 - Dump" << endl;
		cout << "4 - Exit" << endl;	
		cin >> key;
		switch(key){
			case 1:
					sniffer();
					break;
			case 2: 
					crawler();
					break;
			case 3: 
					dump();
					break;
			case 4: 
					exit(0);
			default: 
					cout << "Invalid Key!" << endl;
					exit(0);
		}
		
	}

	return 0;
}

