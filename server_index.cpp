#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <queue>
#include "common_key.h"
#include "server_index.h"

/******************************************************************************
 * 2
 * Federico Manuel Gomez Peter; 19 253
*/

//Index::Index(){}
Index::~Index(){}

Index::Index (std::string& filename) {

    std::cerr << "ENTRO \n";
    std::ifstream file;
    file.open(filename); 
    std::cerr << "ABRO \n";
    std::string line;
    std::getline(file, line, '\n');
    this->serial_number = (uint32_t) atoi(line.c_str());
    
    std::cerr << "Line: " << line << '\n';
    fprintf(stderr,"Serial: %d\n", this->serial_number);
    while (std::getline(file, line, '\n')) {
        std::cerr << "WHILE \n";
        this->parseLine(line);
    }
}

void Index::parseLine(std::string& line) {
    std::queue<std::string> container;
    this->split(line, ';', container);

    std::string subject = container.front();
    std::cerr << "Subject: " << subject << '\n';
    container.pop();
    std::string str_key = container.front();
    container.pop();
    
    this->split(str_key, ' ', container);
    std::string str1 = container.front();
    container.pop();
    std::string str2 = container.front();
    container.pop();
    Key key(str1, str2);
    this->certificates.insert({subject, key}); 
}

void Index::split(std::string& str, char c, std::queue<std::string>& container) {
	std::string buff{""};
	int i = 0;
    while ( str[i] ) {
        if(str[i] == c && buff != "") { 
            container.push(buff); 
            buff = ""; 
        } else {
            buff+=str[i];
        }
        i++;
	}
	if (buff != "") {
        container.push(buff);
    }
}
