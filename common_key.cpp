#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "common_key.h"
#define EXPONENT_LEN 1
#define MODULE_LEN 2
#define MODULE_MAX_VALUE 65535
#define EPONENT_MAX_VALUE 255
#define STR_MOD_MAX_LEN 5
#define STR_EXP_MAX_LEN 3


Key::Key(uint8_t _exponent, uint16_t _module):
    exponent(_exponent), module(_module) {}

Key::Key() {}

void Key::set(std::string filename) {
    std::ifstream file;
    file.open(filename); 
    std::string line;
    bool is_exponent_set = false;
    while (std::getline(file, line, ' ')){
        if (line.length() == 0) continue;
        if (!is_exponent_set){
            this->exponent = (uint8_t) atoi(line.c_str());
            is_exponent_set = true;
        } else {
            this->module = (uint8_t) atoi(line.c_str());
        }
    }
    //file.close();
}


Key::~Key() {}

void Key::send(Socket skt) {
    skt.sendAll(&this->exponent, EXPONENT_LEN);
    skt.sendAll(&this->module, MODULE_LEN);
}
