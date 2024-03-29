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


Key::Key(std::string& _public_exponent, std::string& _module) {
    this->set(_public_exponent, _module);
}

Key::Key() {}

void Key::set(std::string& _public_exponent, std::string& _module){
    int aux;
    std::istringstream pe(_public_exponent);
    pe >> aux;
    this->public_exponent = (uint8_t) aux;
    std::istringstream m(_module);
    m >> aux;
    this->module = (uint16_t) aux;
}

Key::Key(std::string& filename) {
    this->set(filename);
}

void Key::set(std::string& filename) {
    std::ifstream file;
    file.open(filename); 
    if (!file.good()) {
        throw std::runtime_error("Error with file while seting key");
    }
    std::string line;
    int i = 0;
    int aux;
    while (std::getline(file, line, ' ')) {
        if (line.length() == 0) continue;
        if (i == PUBLIC_EXP_POS) {
            std::istringstream pbe(line);
            pbe >> aux;
            this->public_exponent = (uint8_t) aux;
        } else if (i == PRIVATE_EXP_POS) {
            std::istringstream pre(line);
            pre >> aux;
            this->private_exponent = (uint8_t) aux;
        } else if (i == MODULE_POS) {
                std::istringstream m(line);
                m >> aux;
                this->module = (uint16_t) aux;
        }
        i++;
    }
    if (i == MODULE_POS) {
        this->module = this->private_exponent;
    }
    //file.close();
}

Key::Key(const Key &key) {
    this->private_exponent = key.private_exponent;
    this->public_exponent = key.public_exponent;
    this->module = key.module;
}

Key::~Key() {}

void Key::receive(Protocol& protocol) {
    protocol.receive(this->public_exponent);
    protocol.receive(this->module);
}

void Key::send(Protocol& protocol) {
    try {
        protocol.send(this->public_exponent);
        protocol.send(this->module);
    }
    catch(std::runtime_error) {
        throw std::runtime_error("Error while sending key");
    }
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
