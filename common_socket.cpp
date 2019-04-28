#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>

#include <errno.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "common_socket.h"

#define MAX_WAITING_CLIENTS 20
#define UINT8_SIZE 1
#define UINT16_SIZE 2
#define UINT32_SIZE 4


/******************************************************************************
 * TODOS LOS 
 * if (s < 0) {
 *    return 
 * }
 * TIENEN QUE SER EXCEPCIONES EN UN FUTURO.
*/

void Socket::start(const char* host, const char* port, int flag) {
    this->skt = 0; 
    this->current_peerskt = 0;
    
    int s = 0;
    struct addrinfo hints; 
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = flag;   

    s = getaddrinfo(host, port, &hints, &this->result);

    if (s != 0) { 
        //do something
    }
}

Socket::Socket(const char* port) {
    this->start(NULL, port, AI_PASSIVE);
}

Socket::Socket(const char* host, const char* port, int flag) {
    this->start(host, port, AI_PASSIVE);
}

bool Socket::reciveAll(void* buf, size_t len) {    
    memset(buf, 0, len);
    size_t received = 0;
    int status = 0;
    bool is_there_an_error = false;
    char* aux = (char*) buf;
    while (received < len && !is_there_an_error) {
        status = this->reciveSome(&aux[received], len - received);
        if (status == 0) { 
            is_there_an_error = true;
        } else if (status < 0) { 
            is_there_an_error = true;
        } else {
            received += status;
        }
    }
    return is_there_an_error;
}


int Socket::reciveAll(std::string& str) {
    uint32_t len = (uint32_t) str.length();
    this->reciveNumber(&len);
    char c;
    int recived = 0;
    for (size_t i = 0; i < len; ++i){
        recived = recv(this->current_peerskt, &c , 1, MSG_NOSIGNAL);
        str.append(1, c);
    }
    return recived;
}

void Socket::reciveNumber(uint8_t* n) {
    this->reciveAll(n, UINT8_SIZE);
}


void Socket::reciveNumber(uint16_t* n) {
    uint16_t aux;
    this->reciveAll(&aux, UINT16_SIZE);
    *n = htobe16(aux);
}


void Socket::reciveNumber(uint32_t* n) {
    uint32_t aux;
    this->reciveAll(&aux, UINT32_SIZE);
    *n = htobe32(aux);
}

int Socket::reciveSome(void* buf, size_t size) {
    return recv(this->current_peerskt, buf , size, MSG_NOSIGNAL);
}

void Socket::sendNumber(uint8_t n) {
    this->sendAll(&n, UINT8_SIZE);
}

void Socket::sendNumber(uint16_t n) {
    uint16_t aux = htobe16(n);
    this->sendAll(&aux, UINT16_SIZE);
}

void Socket::sendNumber(uint32_t n) {
    uint32_t aux = htobe32(n);
    this->sendAll(&aux, UINT32_SIZE);
}

int Socket::sendAll(std::string str) {  
    uint32_t len = (uint32_t) str.length();
    this->sendNumber(len);
    int sent = 0;
    for (size_t i = 0; i < len && sent >= 0; ++i){
        sent = this->sendAll(&str[i], 1);
    }
    return sent;
}

int Socket::sendAll(void* buf, size_t size) {
    //fprintf(stderr, "Envio\n");
    /*
    char* ptr = (char*) buf;
    for (int i = 0; i<(int)size; i++) {
        fprintf(stderr, "%02X\t", (uint8_t)ptr[i]);
    }
    fprintf(stderr, "\n");
    */
    
    int bytes_sent = 0;
    int s = 0;
    bool is_the_socket_valid = true;
    char* aux = (char*) buf;

    while (bytes_sent < (int)size && is_the_socket_valid) {
        s = send(this->current_peerskt, &aux[bytes_sent], \
                size-bytes_sent, MSG_NOSIGNAL);
        if (s <= 0) {
            printf("Error: %s\n", strerror(errno));
            return -1;
        } else {
            bytes_sent += s;
        }
    }
    return bytes_sent;
}


Socket::~Socket() {
    freeaddrinfo(this->result);
    shutdown(this->skt, SHUT_RDWR);
    close(this->skt);
}


bool Socket::connectWithClients() {
    struct addrinfo *ptr = this->result;
    int s = 0;

    this->skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (this->skt == -1) {
        return false;
    }

    int val = 1;
    s = setsockopt(this->skt, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (s == -1) {
        return false;
    }    

    s = bind(this->skt, ptr->ai_addr, ptr->ai_addrlen);
    if (s == -1) {
        return false;
    }
    //
    s = listen(this->skt, MAX_WAITING_CLIENTS);
    if (s == -1) {
        return false;
    }
    return true;
}

void Socket::disableClient() {
       // shutdown(this->current_peerskt, SHUT_RDWR); 
        //close(this->current_peerskt);
}

//-1 si falla
int Socket::acceptClient(){
    int peerskt = accept(this->skt, NULL, NULL);
    this->current_peerskt = peerskt;
    return peerskt;
}


bool Socket::connectWithServer() {
    int s = 0;
    bool are_we_connected = false;
    struct addrinfo *ptr;

    for (ptr = this->result; ptr != NULL && are_we_connected == false;\
        ptr = ptr->ai_next) {
        this->skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (this->skt == -1) continue;
        s = connect(this->skt, ptr->ai_addr, ptr->ai_addrlen);
        are_we_connected = (s != -1);
    }
    this->current_peerskt = this->skt;

    return are_we_connected;
}

void Socket::disablesSendOperations() {
    shutdown(this->skt, SHUT_WR);
}
