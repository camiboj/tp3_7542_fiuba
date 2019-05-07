#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <queue>
#include <map>
#include "common_key.h"
#include "server_index.h"


Index::~Index() {}

Index::Index(std::string& _filename) : filename(_filename) {
    std::ifstream file;
    file.open(filename); 
    std::string line;
    std::getline(file, line, '\n');
    this->serial_number = (uint32_t) atoi(line.c_str());
    if (!this->serial_number) this->serial_number = 1;
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


bool Index::hasCertificate(std::string& str) {
    std::unique_lock<std::mutex> lock(this->mutex);
    std::map<std::string, Key>::iterator it = this->certificates.find(str);
    bool result = it != this->certificates.end();
    return result;
}

Key Index::findCertificate(Certificate& cartificate) {
    std::unique_lock<std::mutex> lock(this->mutex);
    std::map<std::string, Key>::iterator it = \
        this->certificates.find(cartificate.getSubject());
    return it->second;
}

bool Index::hasCertificate(Certificate& certificate) {
    std::string subj = certificate.getSubject();
    return this->hasCertificate(subj);
}

void Index::saveCertificate(Certificate& certificate) {
    std::unique_lock<std::mutex> lock(this->mutex);
    this->certificates.insert({certificate.getSubject(), certificate.getKey()});
    certificate.addSerialNumber(this->serial_number);
    this->serial_number ++;
}

void Index::eraseCertificate(std::string& str) {
    std::unique_lock<std::mutex> lock(this->mutex);
    this->certificates.erase(str);
}

void Index::eraseCertificate(Certificate& certificate) {
    std::unique_lock<std::mutex> lock(this->mutex);
    this->certificates.erase(certificate.getSubject());
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
    //file.close();
}
