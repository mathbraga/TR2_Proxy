
#include "../lib/HTML_Parser.hpp"
#include "../lib/Spider.hpp"
#include "../lib/Proxy_Server.hpp"
#include "../lib/Request.hpp"
#include "../lib/Response.hpp"
#include "../lib/String_Functions.hpp"

using namespace std;


Spider::Spider(string url){
	string internal = url;
	vector<string> result = String_Functions::split(internal, "//");
	if(result.size()==2){
		if(result[0]!="http:"){
		 valid = false;
		 return;
		}
		internal = result[1];
	}
	if(result.size()>2){
	 	valid = false;
	 	return;
	}
	
	if(internal.find('/')!=0 && internal.find('/')<internal.length()){
		vector<string> result2 = String_Functions::splitOnFirst(internal, "/");
		if(host.length()==0){
			host = result2[0];	
		}else{
			valid = false;
			return;
		}
		root = "/";
		root.append(result2[1]);
	}else if(internal.find('/')>internal.length()){
		root = "/";
		host = internal;
	}else{
		valid = false;
		return;
	}

	valid = true;

};

bool Spider::isValid(string url){
	if(url.size() ==0) return false;
	string internal = url;
	vector<string> result = String_Functions::split(internal, "//");
	if(result.size()==2){
		if(result[0]!="http:") return false;
		internal = result[1];
	}
	if(result.size()>2) return false;
	
	if(internal.find('/')!=0){
		vector<string> result2 = String_Functions::split(internal, "/");
		if(host.length()==0){
			return false;	
		}else{
			if(host!=result2[0]) return false;
		}
	}
	return true;
}

string Spider::parseUrl(string url){
	string parsed("");
	string internal = url;
	vector<string> result = String_Functions::split(internal, "//");
	if(result.size()==2)		
		internal = result[1];
	
	if(internal.find('/')!=0){
		vector<string> result2 = String_Functions::splitOnFirst(internal, "/");
		parsed = "/";
		parsed.append(result2[1]);
	}else{
		parsed = internal;
	}

	if(parsed.find("?")<parsed.length()){
		int ask = parsed.find("?");
		parsed = parsed.substr(0,ask);
	}

	if(parsed!=url) aliases[parsed]=url;
	return parsed;
}

std::string Spider::url2filename(std::string url_in){

	string filename("");
	string url = url_in;
	string slash("-");
	vector<string> splitted = String_Functions::split(url, "/");
	if(splitted.size()==1 && splitted[0].length()==0){
		filename = "root.html";	
		return filename;
	}
	for(int i = 0; i<splitted.size(); i++ ){
		filename.append(splitted[i]);
	}
	if(filename.find(".")>filename.length())
		filename.append(".html");
	else if(filename.find("?")<filename.length()){
		int ask = filename.find("?");
		filename = filename.substr(0,ask);
	}
	return filename;
}


void Spider::crawl(){
	int i = 1;
	visited_urls.clear();
	par_child.clear();
	htmls.clear();
	if(!valid){
		return;
	}
	Proxy_Server proxy = Proxy_Server();
	Request request = Request();
	request.treat();
	set<string> to_request;
	to_request.insert(root);
	do{
		set<string> to_request_next;
		for (std::set<string>::iterator url=to_request.begin(); url!=to_request.end(); ++url){
			if(visited_urls.find(*url)==visited_urls.end()){
				visited_urls.insert(*url);
				request.url = *url;
				request.fields["Host:"] = host;
				string reply = proxy.make_request(request.assembly());
				Response response = Response(reply);
				if(response.status_code=="200 OK"){
					
					htmls[*url] = response.data;

					set<string> all_hrefs = HTML_Parser::get_hrefs(response.data.c_str());
					set<string> local_hrefs;
					for (std::set<string>::iterator it=all_hrefs.begin(); it!=all_hrefs.end(); ++it){
				        if(this->isValid(*it)){
				        	local_hrefs.insert(this->parseUrl(*it));
				        }
		     		}
				    par_child[*url]=local_hrefs;
				    for (std::set<string>::iterator it=local_hrefs.begin(); it!=local_hrefs.end(); ++it)
				        to_request_next.insert(*it);
				}    

			}
		}
		to_request = to_request_next;
	 	i--;
	}while(i>=0);

}


void Spider::printCrawled(){

		cout << "URL crawled:"<<endl<<endl;
		cout << "/" <<host << "/" << " -->";
		int nspaces = host.size()+6;
		set<string> childs = par_child[root];
		set<string> alrd_visited;
		alrd_visited.insert(root);	
		for(set<string>::iterator it = childs.begin(); it!=childs.end(); ++it){
			if(alrd_visited.find(*it)==alrd_visited.end()){
				alrd_visited.insert(*it);
				if(std::next(it)==childs.end()) cout << "\u2514" << *it << " \u2500\u2500\u2500";
				else cout << "\u251c" << *it << " \u2500\u2500\u2500";
				int nspaces2 = (*it).size()+4;
				set<string> granchilds = par_child[*it];	
				for(set<string>::iterator it2 = granchilds.begin(); it2!=granchilds.end(); ++it2){
					if(it2==granchilds.begin())
						cout << "\u252c" << *it2  << endl;
					else if(std::next(it2) == granchilds.end())
						cout << "\u2514" << *it2  << endl;
					else
						cout << "\u251c" << *it2  << endl;
					for(int k =0; k<nspaces; k++) cout << " ";
					if(std::next(it)==childs.end()) cout << " ";
					else cout << "\u2502";
					for(int k =0; k<nspaces2; k++) cout << " ";
				}	
			}
			cout << endl;
			for(int k =0; k<nspaces; k++) cout << " ";
		}
		cout << endl;
	
}


void Spider::dump(){
	this->crawl();
	set<string> to_translate = visited_urls;

	dictionary[root]="index.html";
	to_translate.erase(root);

	for(set<string>::iterator it = to_translate.begin(); it!=to_translate.end(); ++it)
		dictionary[*it]=Spider::url2filename(*it);


	for(std::map<string,string>::iterator it=htmls.begin(); it!=htmls.end(); ++it)
    	for(std::map<string,string>::iterator dt=dictionary.begin(); dt!=dictionary.end(); ++dt){
    		it->second = String_Functions::replace(it->second,"\'","\"");
    		string from("\"");from.append(dt->first.c_str());from.append("\"");
    		string to("\"");to.append(dt->second.c_str());to.append("\"");
    		it->second = String_Functions::replace(it->second,from.c_str(),to.c_str());
    		if(aliases.find(dt->second) != aliases.end()){
    			string from("\"");from.append(aliases[dt->second].c_str());from.append("\"");
    			string to("\"");to.append(dt->second.c_str());to.append("\"");
    			it->second = String_Functions::replace(it->second,from.c_str(),to.c_str());
    		}
    }       	
		
    string command("rm -rf ");
    command.append(host);
	system(command.c_str());
    command.assign("mkdir ");
    command.append(host);
	system(command.c_str());

	for(std::map<string,string>::iterator it=htmls.begin(); it!=htmls.end(); ++it){
		cout << "Saving file: " << dictionary[it->first] << endl;
        String_Functions::string2file(it->second, host.c_str(), dictionary[it->first].c_str());   
    }
    cout << "Files saved in " << host << endl;
   
}