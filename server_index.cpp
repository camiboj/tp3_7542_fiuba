#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <queue>
#include <map>
#include "common_key.h"
#include "server_index.h"
#include "server_inexisting_certificate.h"
#include "server_existing_certificate.h"


Index::~Index() {}

Index::Index(std::string& _filename) : filename(_filename) {
    std::ifstream file;
    file.open(filename); 
    if (!file.good()) {
        throw std::runtime_error("Error with index file");
    }
    std::string line;
    std::getline(file, line, '\n');
    
    int aux;
    if (line.size() != 0) { //if line emptry aux = random
        std::istringstream sn(line);
        sn >> aux; 
        this->serial_number = (uint32_t) aux;
    } else {
        this->serial_number = 1;
    }
    while (std::getline(file, line, '\n')) {
        this->parseLine(line);
    }
}


void split(std::string& str, char c, std::queue<std::string>& container) {
	std::string buff{""};
	int i = 0;
    while (str[i]) {
        if (str[i] == c && buff != "") { 
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

void Index::parseLine(std::string& line) {
    std::queue<std::string> container;
    split(line, ';', container);

    std::string subject = container.front();
    container.pop();
    std::string str_key = container.front();
    container.pop();
    
    split(str_key, ' ', container);
    std::string str1 = container.front();
    container.pop();
    std::string str2 = container.front();
    container.pop();
    Key key(str1, str2);
    this->certificates.insert({subject, key}); 
}


bool Index::has(std::string& str) {
    //std::unique_lock<std::mutex> lock(this->mutex);
    std::map<std::string, Key>::iterator it = this->certificates.find(str);
    bool result = it != this->certificates.end();
    return result;
}

Key Index::find(Certificate& cartificate) {
    std::unique_lock<std::mutex> lock(this->mutex);
    std::map<std::string, Key>::iterator it = \
        this->certificates.find(cartificate.getSubject());
    return it->second;
}

bool Index::has(Certificate& certificate) {
    std::string sbj = certificate.getSubject();
    return this->has(sbj);
}

void Index::save(Certificate& certificate) {
    std::unique_lock<std::mutex> lock(this->mutex);
    if (this->has(certificate)) {
        throw ExistingCertificate("Error saving certificate");
    }
    this->certificates.insert({certificate.getSubject(), certificate.getKey()});
    certificate.addSerial(this->serial_number);
    this->serial_number ++;
}

void Index::putBack(Certificate& certificate, Key& client_key) {
    std::unique_lock<std::mutex> lock(this->mutex);
    this->certificates.insert({certificate.getSubject(), client_key});
}

void Index::erase(std::string& str) {
    std::unique_lock<std::mutex> lock(this->mutex);
    if (!this->has(str)) {
        throw InexistingCertificate("Error erasing certificate");
    }
    this->certificates.erase(str);
}

void Index::erase(Certificate& certificate) {
    std::string subj = certificate.getSubject();
    try {
        this->erase(subj);
    }
    catch(InexistingCertificate) {
        __throw_exception_again;
    }
}

void Index::write() {
    std::ofstream file;
    file.open(this->filename, std::ofstream::out | std::ofstream::trunc);
    
    if (!file.is_open()) {
        //exc
    }
    file << std::to_string(this->serial_number) << '\n';
    std::map<std::string, Key>::iterator it = this->certificates.begin();
    for (; it != this->certificates.end(); ++it) {
        file << it->first << "; " << it->second << '\n';
    }
}
