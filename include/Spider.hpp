#ifndef SPIDER_H
#define SPIDER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>

/*
Headers da Classe Spider
*/
class Spider{

public:

	Spider(std::string);
	bool valid;
	std::string root; // url raiz do spider
	std::string host; // host do spider
	
	// urls já visitadas pra evitar loop infinito
	std::set<std::string> visited_urls; 
	
	// liga url a conteúdo html das respostas
	std::map<std::string, std::string> htmls; 
	
	// dicionario de urls para filenames equivalentes para substituir no html
	std::map<std::string, std::string> dictionary; 
	
	// relaciona urls para depois substituir no html 
	std::map<std::string, std::string> aliases; 
	
	// estrutura utilizada para representar a árvore (url_pai->lista de filhos)
	std::map<std::string, std::set<std::string>> par_child; 
	


	bool eval_url(std::string url);
	std::string parse_url(std::string url);
	std::string url_to_filename(std::string url);
	void generate_tree(int);
	void print_tree(int);
	void dump(int);

};


#endif