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

Socket::Socket(int _skt, int _current_peerskt) : 
skt(_skt), 
current_peerskt(_current_peerskt) {}

Socket::Socket(int _current_peerskt) : current_peerskt(_current_peerskt) {}

/*Socket::Socket(int _skt, int _current_peerskt, struct addrinfo *_result) {
    this->skt = _skt;
    this->current_peerskt = _current_peerskt;
    this->result = _result;
}*/

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

int Socket::reciveSome(void* buf, size_t size) {
    return recv(this->current_peerskt, buf , size, MSG_NOSIGNAL);
}

int Socket::reciveSome(void* buf, size_t size, int peerskt) {
    return recv(peerskt, buf , size, MSG_NOSIGNAL);
}

int Socket::sendAll(void* buf, size_t size) {    
    int bytes_sent = 0;
    int s = 0;
    bool is_the_socket_valid = true;
    char* aux = (char*) buf;

    while (bytes_sent < (int)size && is_the_socket_valid) {
        s = send(this->current_peerskt, &aux[bytes_sent], \
                size-bytes_sent, MSG_NOSIGNAL);
        if (s <= 0) {
            //printf("Error: %s\n", strerror(errno));
            return -1;
        } else {
            bytes_sent += s;
        }
    }
    return bytes_sent;
}


Socket::~Socket() {
    if (this->skt == -1) return; 
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
    freeaddrinfo(this->result);
}

void Socket::disableClient() {
    shutdown(this->current_peerskt, SHUT_RDWR); 
    close(this->current_peerskt);
}

Socket Socket::acceptClient(){
    int peerskt = accept(this->skt, NULL, NULL);
    Socket skt(peerskt);
    return std::move(skt);
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

void Socket::kill() {
    shutdown(this->skt, SHUT_RDWR);
    close(this->skt);
}