#include "common_my_socket.h"
#include <iostream>
#define UINT8_SIZE 1
#define UINT16_SIZE 2
#define UINT32_SIZE 4

MySocket::MySocket(Socket& _skt): skt(_skt) {}

MySocket::~MySocket() {}

void MySocket::sendNumber(uint8_t* n) {
    this->skt.sendAll(n, UINT8_SIZE);
}

void MySocket::sendNumber(uint16_t* n) {
    uint16_t aux = htobe16(*n);
    this->skt.sendAll(&aux, UINT16_SIZE);
}

void MySocket::sendNumber(uint32_t* n) {
    uint32_t aux = htobe32(*n);
    this->skt.sendAll(&aux, UINT32_SIZE);
}

int MySocket::sendAll(std::string str) { 
    //std::cout << "Str sent: " << str; 
    uint32_t len = (uint32_t) str.length();
    this->sendNumber(&len);
    int sent = 0;
    for (size_t i = 0; i < len && sent >= 0; ++i){
        sent = this->skt.sendAll(&str[i], 1);
    }
    return sent;
}


int MySocket::reciveAll(std::string& str) {
    uint32_t len = (uint32_t) str.length();
    this->reciveNumber(&len);
    char c;
    int recived = 0;
    for (size_t i = 0; i < len; ++i){
        recived = this->skt.reciveSome(&c, 1);
        str.append(1, c);
    }
    return recived;
}

void MySocket::reciveNumber(uint8_t* n) {
    this->skt.reciveAll(n, UINT8_SIZE);
}


void MySocket::reciveNumber(uint16_t* n) {
    uint16_t aux;
    this->skt.reciveAll(&aux, UINT16_SIZE);
    *n = htobe16(aux);
}


void MySocket::reciveNumber(uint32_t* n) {
    uint32_t aux;
    this->skt.reciveAll(&aux, UINT32_SIZE);
    *n = htobe32(aux);
}

void MySocket::stop() {
    this->skt.disableClient();
}