#ifndef SERVER_CLIENT_PROCESSOR_H
#define SERVER_CLIENT_PROCESSOR_H
#include "server_thread.h"


class ClientProcessor : public Thread {
    public:
        ClientProcessor();
        ~ClientProcessor();
};

#endif
