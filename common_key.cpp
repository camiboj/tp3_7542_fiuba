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

/*
Key::Key(uint8_t _exponent, uint16_t _module):
    exponent(_exponent), module(_module) {}
*/
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


Key::~Key() {}

void Key::recive(Socket& skt) {
    skt.reciveSome(&this->public_exponent, EXPONENT_LEN);
    uint16_t aux;
    skt.reciveSome(&aux, MODULE_LEN);
    this->module = htobe16(aux);
    std::cerr << "KEY:\t";
    fprintf(stderr,"< %d > < %d >\n", this->public_exponent, this->module);
}

void Key::send(Socket& skt) {
    skt.sendAll(&this->public_exponent, EXPONENT_LEN);
    
    uint16_t aux = htobe16(this->module);
    skt.sendAll(&aux, MODULE_LEN);
}

/*********  PRINT   **************************
    std::cerr << "\tKEY:\n";
    fprintf(stderr,"< %d > < %d >\n", this->public_exponent, this->module);
    
*/
