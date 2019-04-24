#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

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
 * TIENEN QUE SER EXCEPSIONES EN UN FUTURO.
*/

void Socket::start(const char* host, const char* port) {
    this->skt = 0; 
    this->current_peerskt = 0;
    
    int s = 0;
    struct addrinfo hints; 
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = 0;   

    s = getaddrinfo(host, port, &hints, &this->result);

    if (s != 0) { 
        //do something
    }
}

Socket::Socket(const char* port) {
    this->start(NULL, port);
}

Socket::Socket(const char* host, const char* port) {
    this->start(host, port);
}

bool Socket::reciveAll(void* buf, size_t len) {    
    memset(buf, 0, len);
    size_t received = 0;
    int status = 0;
    bool is_there_an_error = false;
    char* aux = (char*) buf;
    while (received < len && !is_there_an_error) {
        status = this->receiveSome(&aux[received], len - received);
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


int Socket::receiveSome(void* buf, size_t size) {
    return recv(this->current_peerskt, buf , size, MSG_NOSIGNAL);
}

int Socket::sendAll(std::string buf, size_t size) {  
    // Opcion 1: strcopy
    // Opcion 2: repetir el código de la de abajo.  
    int sent = 0;
    for (size_t i = 0; i < size && sent > 0; ++i){
        sent = this->sendAll(&buf[i], size);
    }
    return sent;
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
        shutdown(this->current_peerskt, SHUT_RDWR); 
        close(this->current_peerskt);
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
