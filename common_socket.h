#ifndef COMMON_SOCKET_H
#define COMMON_SOCKET_H
#include <stdlib.h>
#include <stdbool.h>

class Socket {
    private:    
        int skt;

    public:
        Socket();
        ~Socket();

        Socket(int skt);
        Socket(Socket&& origin); 
        Socket& operator=(Socket&& origin);

        void connectWithClients(const char* port);
        Socket acceptClient();

        bool connectWithServer(const char* host, const char* port);


        bool receiveAll(void* buf, size_t len);
        int receiveSome(void* buf, size_t size);
        int sendAll(void* buf, size_t size);

        void kill();
};


#endif
