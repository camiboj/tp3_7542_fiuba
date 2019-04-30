#ifndef COMMON_SOCKET_H
#define COMMON_SOCKET_H
#include <stdlib.h>
#include <stdbool.h>
#include <string>

class Socket {
    private:
        int skt;
        int current_peerskt;
        struct addrinfo *result;
        void start(const char* _host, const char* _port, int flag);
        Socket(int skt, int current_peerskt, struct addrinfo *result);

    public:
        /*
        * Crea e incializa el socket definiendo la familia, el tipo de socket y el 
        * protocolo para poder conectarse al cliente por medio del port y host indicados
        */
        Socket(const char* _host, const char* _port, int flag);
        
        explicit Socket(const char* _port);
         Socket(int _currentpeerskt);
        Socket(int skt, int _currentpeerskt);
        ~Socket();

        /*
        * Almacena los parametros necesarios para la incialización del socket.
        */
        bool connectWithClients();

        Socket acceptClient();

        //void reciveNumber(uint8_t* n);
        //void reciveNumber(uint16_t* n);
        //void reciveNumber(uint32_t* n);
        
        bool reciveAll(void* buf, size_t len);
        //int reciveAll(std::string& str);
        int reciveSome(void* buf, size_t size);
        int reciveSome(void* buf, size_t size, int peerskt);


        int sendAll(void* buf, size_t size);
        //int sendAll(std::string str);
        
        /*
        * Desactiva las operaciones de envío y recepción para el cliente y para si mismo
        */
        void disableClient();

        /*
        * Desabilita el canal de escritura
        */
        void disablesSendOperations();
        bool connectWithServer();
        void kill();
};

#endif
