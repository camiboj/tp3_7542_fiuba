#ifndef COMMON_MY_SOCKET_H
#define COMMON_MY_SOCKET_H
#include "common_socket.h"

/*
 * Encapsulación del socket que conoce el protocolo de comunicación pedido
*/
class MySocket {
    private:
        Socket& skt;
    public:
        MySocket(Socket& _skt);
        ~MySocket();

        void reciveNumber(uint8_t* n);
        void reciveNumber(uint16_t* n);
        void reciveNumber(uint32_t* n);

        void sendNumber(uint8_t* n);
        void sendNumber(uint16_t* n);
        void sendNumber(uint32_t* n);

        int sendAll(std::string str);
        int reciveAll(std::string& str);
        void stop();
};

#endif