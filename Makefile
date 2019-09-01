all: aracne

HTML_Parser.o:
	g++ -g -c -std=c++11 src/HTML_Parser.cpp

Request.o:
	g++ -g -c -std=c++11 src/Request.cpp

Response.o:
	g++ -g -c -std=c++11 src/Response.cpp

Proxy_Server.o: 
	g++ -g -c -std=c++11 src/Proxy_Server.cpp 

Spider.o:
	g++ -g -c -std=c++11 src/Spider.cpp 

String_Functions.o:
	g++ -g -c -std=c++11 src/String_Functions.cpp 


aracne: HTML_Parser.o Proxy_Server.o Spider.o String_Functions.o Response.o Request.o src/main.cpp
	g++ -Wall -std=c++11 -o aracne HTML_Parser.o Spider.o Request.o Response.o Proxy_Server.o String_Functions.o src/main.cpp

clean: 
	rm aracne HTML_Parser.o Proxy_Server.o Request.o Spider.o Response.o String_Functions.o
