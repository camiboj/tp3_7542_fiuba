#include "common_protocol.h"
#include <string>
#include <iostream>
#define UINT8_SIZE 1
#define UINT16_SIZE 2
#define UINT32_SIZE 4

Protocol::Protocol(Socket& _skt) {
    skt = std::move(_skt);
}

Protocol::Protocol() {}

Protocol::~Protocol() {}

void Protocol::send(uint8_t& n) {
    try {
        this->skt.sendAll(&n, UINT8_SIZE);
    }
    catch(std::runtime_error &e) {
        throw std::runtime_error("Error in protocol while sending");
    }
}

void Protocol::send(uint16_t& n) {
    uint16_t aux = htobe16(n);
    try {
        this->skt.sendAll(&aux, UINT16_SIZE);
    }
    catch(std::runtime_error &e) {
        throw std::runtime_error("Error in protocol while sending");
    }
}

void Protocol::send(uint32_t& n) {
    uint32_t aux = htobe32(n);
    try {
        this->skt.sendAll(&aux, UINT32_SIZE);
    }
    catch(std::runtime_error &e) {
        throw std::runtime_error("Error in protocol while sending");
    }
}

int Protocol::send(std::string& str) { 
    uint32_t len = (uint32_t) str.length();
    int sent = 0;
    try {    
        this->send(len);
        for (size_t i = 0; i < len && sent >= 0; ++i){
            sent = this->skt.sendAll(&str[i], 1);
        }
    }
    catch(std::runtime_error &e) {
        throw std::runtime_error("Error in protocol while sending");
    }
    return sent;
}


int Protocol::receive(std::string& str) {
    uint32_t len = (uint32_t) str.length();
    this->receive(len);
    char c;
    int received = 0;
    for (size_t i = 0; i < len; ++i){
        received = this->skt.receiveSome(&c, 1);
        str.append(1, c);
    }
    return received;
}

void Protocol::receive(uint8_t& n) {
    this->skt.receiveAll(&n, UINT8_SIZE);
}


void Protocol::receive(uint16_t& n) {
    uint16_t aux;
    this->skt.receiveAll(&aux, UINT16_SIZE);
    n = htobe16(aux);
}


void Protocol::receive(uint32_t& n) {
    uint32_t aux;
    this->skt.receiveAll(&aux, UINT32_SIZE);
    n = htobe32(aux);
}

void Protocol::stop() {}

Protocol& Protocol::operator=(Protocol&& origin) {
    this->skt = std::move(origin.skt);
    return *this;
}

Protocol::Protocol(Protocol&& origin) {
    this->skt = std::move(origin.skt);
    origin.skt.setInvalid();
}
