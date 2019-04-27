#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "common_key.h"
#define EXPONENT_SIZE 1
#define MODULE_SIZE 2
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

Key::Key(const Key &key) {
    this->private_exponent = key.private_exponent;
    this->public_exponent = key.public_exponent;
    this->module = key.module;
}

Key::~Key() {}

void Key::recive(Socket& skt) {
    skt.reciveSome(&this->public_exponent, EXPONENT_SIZE);
    uint16_t aux;
    skt.reciveSome(&aux, MODULE_SIZE);
    this->module = htobe16(aux);
}

void Key::send(Socket& skt) {
    skt.sendAll(&this->public_exponent, EXPONENT_SIZE);
    
    uint16_t aux = htobe16(this->module);
    skt.sendAll(&aux, MODULE_SIZE);
}

std::ostream& operator<<(std::ostream& o, const Key& self) {
    std::string s = std::to_string(self.public_exponent);
    o << s << ' ';
    s = std::to_string(self.module);
    o << s;
    return o;
}

void Key::printPublicExponent(std::string& o, \
                    std::function<std::string(int, int)> transform) {
    int exp = (int) this->public_exponent;
    std::string transformed_exp = transform(exp, EXPONENT_SIZE);
    o += std::to_string(exp) + ' ' + transformed_exp;
}

void Key::printModule(std::string& o, \
                    std::function<std::string(int, int)> transform) {
    int module = (int) this->module;
    std::string transformed_module = transform(module, MODULE_SIZE);
    o += std::to_string(module) + ' ' + transformed_module;
}


uint8_t Key::getPublicExponent() {
    return this->public_exponent;
}
uint8_t Key::getPrivateExponent() {
    return this->private_exponent;
}
uint16_t Key::getModule() {
    return this->module;
}

/*********  PRINT   **************************
    std::cerr << "\tKEY:\n";
    fprintf(stderr,"< %d > < %d >\n", this->public_exponent, this->module);
    
*/
