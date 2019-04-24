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
        void start(const char* _host, const char* _port);

    public:
        /*
        * Crea e incializa el socket definiendo la familia, el tipo de socket y el 
        * protocolo para poder conectarse al cliente por medio del port y host indicados
        */
        Socket(const char* _host, const char* _port);
        Socket(const char* _port);
        ~Socket();

        /*
        * Almacena los parametros necesarios para la incialización del socket.
        */

        bool connectWithClients();

        int acceptClient();

        
        bool reciveAll(void* buf, size_t len);
        int reciveSome(std::string& str, size_t size);
        int reciveSome(void* buf, size_t size);

        int sendAll(void* buf, size_t size);
        int sendAll(std::string buf, size_t size);
        
        /*
        * Desactiva las operaciones de envío y recepción para el cliente y para si mismo
        */
        void disableClient();

        /*
        * Desabilita el canal de escritura
        */
        void disablesSendOperations();
        bool connectWithServer();
};

#endif
