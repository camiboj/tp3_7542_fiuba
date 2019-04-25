#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "common_key.h"
#define EXPONENT_LEN 1
#define MODULE_LEN 2
#define PUBLIC_EXP_POS 0
#define PRIVATE_EXP_POS 1
#define MODULE_POS 2


Key::Key(std::string _public_exponent, std::string _module) {
    this->public_exponent = (uint8_t) atoi(_public_exponent.c_str());
    this->module = (uint8_t) atoi(_module.c_str());
}

Key::Key() {}

void Key::set(std::string filename) {
    std::ifstream file;
    file.open(filename); 
    std::string line;
    int i = 0;
    while (std::getline(file, line, ' ')) {
        if (line.length() == 0) continue;
        if (i == PUBLIC_EXP_POS) {
            this->public_exponent = (uint8_t) atoi(line.c_str());
        } else if (i == PRIVATE_EXP_POS) {
            this->private_exponent = (uint8_t) atoi(line.c_str());
        } else if (i == MODULE_POS) {
            this->module = (uint16_t) atoi(line.c_str());
        }
        i++;
    }
    //file.close();
}

Key::Key (const Key &key) {
    this->private_exponent = key.private_exponent;
    this->public_exponent = key.public_exponent;
    this->module = key.module;
}

Key::~Key() {}

void Key::recive(Socket& skt) {
    skt.reciveSome(&this->public_exponent, EXPONENT_LEN);
    uint16_t aux;
    skt.reciveSome(&aux, MODULE_LEN);
    this->module = htobe16(aux);
    std::cerr << "KEY in reive:\t";
    fprintf(stderr,"< %d > < %d >\n", this->public_exponent, this->module);
}

void Key::send(Socket& skt) {
    skt.sendAll(&this->public_exponent, EXPONENT_LEN);
    
    uint16_t aux = htobe16(this->module);
    skt.sendAll(&aux, MODULE_LEN);
}

std::ostream& operator<<(std::ostream& o, const Key& self) {
    std::cerr << "KEY operator << :\t";
    fprintf(stderr,"< %d > < %d >\n", self.public_exponent, self.module);
    std::string s = std::to_string(self.public_exponent);
    //std::cerr << "S: " << s << '\n';
    o << s << ' ';
    s = std::to_string(self.module);
    //std::cerr << "S: " << s << '\n';
    o << s;
    return o;
}
/*********  PRINT   **************************
    std::cerr << "\tKEY:\n";
    fprintf(stderr,"< %d > < %d >\n", this->public_exponent, this->module);
    
*/
