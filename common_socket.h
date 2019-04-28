#ifndef COMMON_SOCKET_H
#define COMMON_SOCKET_H
#include <stdlib.h>
#include <stdbool.h>
#include <string>

class Socket {
    private:
        const char* host;
        const char* port;
        int skt;
        int current_peerskt;
        struct addrinfo *result;
        void start(const char* _host, const char* _port, int flag);

    public:
        /*
        * Crea e incializa el socket definiendo la familia, el tipo de socket y el 
        * protocolo para poder conectarse al cliente por medio del port y host indicados
        */
        Socket(const char* _host, const char* _port, int flag);
        explicit Socket(const char* _port);
        ~Socket();

        /*
        * Almacena los parametros necesarios para la incialización del socket.
        */

        bool connectWithClients();

        int acceptClient();

        void reciveNumber(uint8_t* n);
        void reciveNumber(uint16_t* n);
        void reciveNumber(uint32_t* n);
        
        bool reciveAll(void* buf, size_t len);
        int reciveAll(std::string& str);
        int reciveSome(void* buf, size_t size);

        void sendNumber(uint8_t n);
        void sendNumber(uint16_t n);
        void sendNumber(uint32_t n);
        int sendAll(void* buf, size_t size);
        int sendAll(std::string str);
        
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
