#include <endian.h>
#include <iostream>
#include <string>
#include "common_string.h"
#define LEN_SIZE 4


String::String(std::string& _str): str(_str) {
    //std::cerr << "String: " << str << '\n';
    this->len = (uint32_t) str.length();
    //this->len = htobe32(aux);
}
void String::recive(Socket& skt) {
    skt.reciveAll(this->str);
}

void String::send(Socket& skt) {
    skt.sendAll(this->str);
}

String::~String() {}

