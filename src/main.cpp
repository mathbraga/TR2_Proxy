/*
	Classe principal que implementa a Command Line Interface, que printa menus e define a excução do Aracne

*/

#include <iostream>
#include "../lib/HTML_Parser.hpp"
#include "../lib/Spider.hpp"
#include "../lib/Proxy_Server.hpp"
#include "../lib/HTTP_Request.hpp"
#include "../lib/HTTP_Response.hpp"
#include "../lib/String_Functions.hpp"

using namespace std;

int port;

void cmd_input(int argc, char **argv){
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


int traffic_inspector(){
	Proxy_Server server = Proxy_Server();
	server.init(port);
	system("clear");
	cout << "Traffic inspector is listening on port " << port << endl;
	cout << "Waiting for Request..." << endl;	
	while(1){
		system("clear");
		cout << "Traffic inspector is listening on port " << port << endl;
		cout << "Waiting for Request..." << endl;
		string req = server.get_client_request();
		HTTP_Request request = HTTP_Request(req);
		cout << "Request received - Host: " << request.fields["Host:"] << " URL: " << request.url << endl;
		if(request.eval()){
			String_Functions::string_to_file(req, ".", "request.txt");
			system("nano request.txt");
			req = String_Functions::string_from_file("request.txt");
			string reply = server.make_request(req);
			HTTP_Response response = HTTP_Response(reply);
			String_Functions::string_to_file(reply, ".", "response.txt");
			system("nano response.txt");
			reply = String_Functions::string_from_file("response.txt");
			server.reply_client(reply);
			system("clear");
			return(0);
		}else{
			cout <<endl<< "Error!" << endl;
		}
	}
}


void gen_tree(){
	string url;
	system("clear");
	cout << "Generate Tree" << endl;
	cout << "Type the url you want to inspect: ";
	cin >> url;
	Spider spider = Spider(url);

	while(!spider.valid){
		system("clear");
		cout << "You have typed an invalid url!" << endl;
		cout << "Generate Tree" << endl;
		cout << "Type the url you want to inspect: ";
		cin >> url;
		spider = Spider(url);
	}
	
	spider.generate_tree(2);
	spider.print_tree(2);
	cout << "Press any key to go back to main menu...";
	std::cin.ignore();
}


void dump(){
	
	string url;
	system("clear");
	cout << "Dump website" << endl;
	cout << "Type the url you want to dump: ";
	cin >> url;
	Spider spider = Spider(url);

	while(!spider.valid){
		system("clear");
		cout << "You have typed an invalid url!" << endl;
		cout << "Dump website" << endl;
		cout << "Type the url you want to dump= ";
		cin >> url;
		spider = Spider(url);
	}
	int lev=0;
	while(lev<1 || lev>2){
		system("clear");
		string levels;
		cout << "Type the number of levels you want to inspect [0 < l < 4]= " ;
		cin >> levels;
		lev = atoi(levels.c_str());
	}
	spider.dump(lev-1);

	string temp;
	cout << "Press any key to go back to main menu...";
	cin >> temp;

}

int main(int argc, char **argv){

	int key;

	cmd_input(argc, argv);
	while(key!=4){
		system("clear");
		cout << "Select an Option: " << endl;
		cout << "1 - Sniff" << endl;
		cout << "2 - Spider" << endl;
		cout << "3 - Dump" << endl;
		cout << "4 - Exit" << endl;	
		cin >> key;
		switch(key){
			case 1:
					traffic_inspector();
					break;
			case 2: 
					gen_tree();
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

