#ifndef COMMON_MY_SOCKET_H
#define COMMON_MY_SOCKET_H
#include "common_socket.h"
#include <string>

/*
 * Encapsulación del socket que conoce el protocolo de comunicación pedido
*/
class Protocol {
    private:
        Socket skt;

    public:
        explicit Protocol(Socket _skt);
        ~Protocol();

        void receiveNumber(uint8_t* n);
        void receiveNumber(uint16_t* n);
        void receiveNumber(uint32_t* n);

        void sendNumber(uint8_t* n);
        void sendNumber(uint16_t* n);
        void sendNumber(uint32_t* n);

        int sendAll(std::string& str);
        int receiveAll(std::string& str);
        void stop();
};

#endif
