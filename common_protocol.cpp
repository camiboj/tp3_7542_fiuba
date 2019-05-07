#include "common_protocol.h"
#include <string>
#include <iostream>
#define UINT8_SIZE 1
#define UINT16_SIZE 2
#define UINT32_SIZE 4

Protocol::Protocol(Socket _skt) {
    skt = std::move(_skt);
}

Protocol::~Protocol() {}

void Protocol::sendNumber(uint8_t* n) {
    this->skt.sendAll(n, UINT8_SIZE);
}

void Protocol::sendNumber(uint16_t* n) {
    uint16_t aux = htobe16(*n);
    this->skt.sendAll(&aux, UINT16_SIZE);
}

void Protocol::sendNumber(uint32_t* n) {
    uint32_t aux = htobe32(*n);
    this->skt.sendAll(&aux, UINT32_SIZE);
}

int Protocol::sendAll(std::string& str) { 
    //std::cout << "Str sent: " << str; 
    uint32_t len = (uint32_t) str.length();
    this->sendNumber(&len);
    int sent = 0;
    for (size_t i = 0; i < len && sent >= 0; ++i){
        sent = this->skt.sendAll(&str[i], 1);
    }
    return sent;
}


int Protocol::receiveAll(std::string& str) {
    uint32_t len = (uint32_t) str.length();
    this->receiveNumber(&len);
    char c;
    int received = 0;
    for (size_t i = 0; i < len; ++i){
        received = this->skt.receiveSome(&c, 1);
        str.append(1, c);
    }
    return received;
}

void Protocol::receiveNumber(uint8_t* n) {
    this->skt.receiveAll(n, UINT8_SIZE);
}


void Protocol::receiveNumber(uint16_t* n) {
    uint16_t aux;
    this->skt.receiveAll(&aux, UINT16_SIZE);
    *n = htobe16(aux);
}


void Protocol::receiveNumber(uint32_t* n) {
    uint32_t aux;
    this->skt.receiveAll(&aux, UINT32_SIZE);
    *n = htobe32(aux);
}

void Protocol::stop() {}
