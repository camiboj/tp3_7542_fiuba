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

//+++++++ IGUAL ++++++++++

int Socket::receive_some(char* buf, size_t size) {
    return recv(this->current_peerskt, buf , size, MSG_NOSIGNAL);
}

int Socket::send_all(std::string buf, size_t size) {  
    // es igual al de abajo pero sino puedo copiar el string en char* y llamar
    // al de abajo.... pero copio al pedo
    int bytes_sent = 0;
    int s = 0;
    bool is_the_socket_valid = true;

    while (bytes_sent < size && is_the_socket_valid) {
        s = send(this->current_peerskt, &buf[bytes_sent], \
                size-bytes_sent, MSG_NOSIGNAL);
        if (s <= 0) {
            return -1;
        } else {
            bytes_sent += s;
        }
    }
    return bytes_sent;
}

int Socket::send_all(void* buf, size_t size) {
    int bytes_sent = 0;
    int s = 0;
    bool is_the_socket_valid = true;
    char* aux = (char*) buf;

    while (bytes_sent < size && is_the_socket_valid) {
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


Socket::Socket(const char* _host, const char* _port) {
    this->host = _host;
    this->port = _port;
    this->skt = 0; 
    this->current_peerskt = 0;
}

Socket::~Socket() {
    freeaddrinfo(this->result);
    shutdown(this->skt, SHUT_RDWR);
    close(this->skt);
}

bool Socket::start() {
    int s = 0;

    struct addrinfo hints; 

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = 0;   

    s = getaddrinfo(this->host, this->port, &hints, &this->result);

    if (s != 0) { 
        return false;
    }
    return true;
}



//+++++ UNICO ++++++++

bool Socket::connect_with_client() {
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

void Socket::disable_client() {
        shutdown(this->current_peerskt, SHUT_RDWR); 
        close(this->current_peerskt);
}

//-1 si falla
int Socket::accept_client(){
    int peerskt = accept(this->skt, NULL, NULL);
    this->current_peerskt = peerskt;
    return peerskt;
}


//+++++ UNICO ++++++++

bool Socket::connect_with_server() {
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

void Socket::disables_send_operations() {
    shutdown(this->skt, SHUT_WR);
}
