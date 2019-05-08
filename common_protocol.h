#ifndef COMMON_PROTOCOL_H
#define COMMON_PROTOCOL_H
#include "common_socket.h"
#include <string>

/*
 * Encapsulación del socket que conoce el protocolo de comunicación pedido
*/
class Protocol {
    private:
        Socket skt;

    public:
        Protocol();
        explicit Protocol(Socket& _skt);
        ~Protocol();
        Protocol(Protocol&& origin); 
        Protocol& operator=(Protocol&& origin);
        void receive(uint8_t& n);
        void receive(uint16_t& n);
        void receive(uint32_t& n);

        void send(uint8_t& n);
        void send(uint16_t& n);
        void send(uint32_t& n);

        int send(std::string& str);
        int receive(std::string& str);
        void stop();
};

#endif
