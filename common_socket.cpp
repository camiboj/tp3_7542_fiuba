#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <utility>
#include <stdexcept>
#include <iostream>
#include "common_socket.h"

#define MAX_WAITING_CLIENTS 20

/******************************************************************************
 * TODOS LOS 
 * if (s < 0) {
 *    return 
 * }
 * TIENEN QUE SER EXCEPCIONES EN UN FUTURO.
*/



Socket::Socket() {
    int familia_skt = AF_INET;      /* IPv4 (or AF_INET6 for IPv6)     */
    int tipo_skt = SOCK_STREAM;     /* TCP  (or SOCK_DGRAM for UDP)    */
    int protocolo_skt = 0;          /* Any protocol */
    int s = socket(familia_skt, tipo_skt, protocolo_skt);

    if (!s) {
        //throw
    }
    this->skt = s;
}

Socket::Socket(int skt) {
    if (skt == -1) {
        throw std::runtime_error("Error accepting client");
    }
    this->skt = skt;
}


Socket& Socket::operator=(Socket&& origin) {
    if (this->skt != -1) {
        //std::cout << "Copiando socket referencia move (?)" << std::endl; 
        shutdown(this->skt, SHUT_RDWR);
        close(this->skt);
    }
    
    this->skt = origin.skt;
    origin.skt = -1;
    return *this;
}

Socket::Socket(Socket&& origin): skt(origin.skt) {
    //std::cout << "Origin.skt = -1 WOWOWOWO" << std::endl; 
    origin.skt = -1;
}


void Socket::kill(){
    if (this->skt != -1) {
        //std::cout << "Cerrando posta con KILL" << std::endl; 
        shutdown(this->skt, SHUT_RDWR);
        close(this->skt);
        this->skt = -1;
    }
}

Socket::~Socket() {
    //std::cout << "Destruyo socket básico: " << std::endl;
    if (this->skt != -1) {
        //std::cout << "Cerrando posta" << std::endl; 
        shutdown(this->skt, SHUT_RDWR);
        close(this->skt);
    }
}


void Socket::connectWithClients(const char* port) {
    struct addrinfo hints;
    struct addrinfo *results, *ptr;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;          /* IPv4 (or AF_INET6 for IPv6)     */
    hints.ai_socktype = SOCK_STREAM;    /* TCP  (or SOCK_DGRAM for UDP)    */
    hints.ai_flags = AI_PASSIVE;        /* AI_PASSIVE for server           */

    int s = 0;
    s = getaddrinfo(NULL, port, &hints, &results);

    if (s != 0) {
        //trow
        return;
    }

    int val = 1;
    s = setsockopt(this->skt, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (s == -1) {
        //trow
        return;
    }

    ptr = results;
    s = bind(this->skt, ptr->ai_addr, ptr->ai_addrlen);
    if (s == -1) {
        //trow
        return;
    }
    freeaddrinfo(results);
    s = listen(this->skt, MAX_WAITING_CLIENTS);
    if (s == -1) {
        //trow
        return;
    }
    return;
}


bool Socket::connectWithServer(const char* host, const char* port) {
    struct addrinfo hints;
    struct addrinfo *result, *ptr;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;          /* IPv4 (or AF_INET6 for IPv6)     */
    hints.ai_socktype = SOCK_STREAM;    /* TCP  (or SOCK_DGRAM for UDP)    */
    hints.ai_flags = 0;                 /* None                            */

    int s = 0;
    s = getaddrinfo(host, port, &hints, &result);
    
    bool are_we_connected = false;

    for (ptr = result; ptr != NULL && are_we_connected == false;\
        ptr = ptr->ai_next) {
        s = connect(this->skt, ptr->ai_addr, ptr->ai_addrlen);
        are_we_connected = (s != -1);
    }
    freeaddrinfo(result);

    return are_we_connected;
}


bool Socket::receiveAll(void* buf, size_t len) {    
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
    return ::recv(this->skt, buf , size, MSG_NOSIGNAL);
}

int Socket::sendAll(void* buf, size_t size) {    
    int bytes_sent = 0;
    int s = 0;
    bool is_the_socket_valid = true;
    char* aux = (char*) buf;

    while (bytes_sent < (int)size && is_the_socket_valid) {
        s = ::send(this->skt, &aux[bytes_sent], \
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


Socket Socket::acceptClient(){
    int peerskt = accept(this->skt, NULL, NULL);
    Socket skt(peerskt);
    return std::move(skt);
}

