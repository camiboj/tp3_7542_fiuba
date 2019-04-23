#ifndef COMMON_SOCKET_H
#define COMMON_SOCKET_H
#include <stdlib.h>
#include <stdbool.h>

class Socket {
    private:
        const char* host;
        const char* port;
        int skt;
        int current_peerskt;
        struct addrinfo *result;

    public:
        /*
        * Crea e incializa el socket definiendo la familia, el tipo de socket y el 
        * protocolo para poder conectarse al cliente por medio del port y host indicados
        */
        Socket(const char* _host, const char* _port);
        ~Socket();

        /*
        * Almacena los parametros necesarios para la incialización del socket.
        */
        bool start();

        bool connect_with_client();

        int accept_client();

        int receive_some(char* buf, size_t size);

        int send_all(void* buf, size_t size);
        int Socket::send_all(std::string buf, size_t size)
        
        /*
        * Desactiva las operaciones de envío y recepción para el cliente y para si mismo
        */
        void disable_client();

        /*
        * desabilita el canal de escritura
        */
        void disables_send_operations();
        bool connect_with_server();
};

#endif
