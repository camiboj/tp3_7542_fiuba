#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H
#include <string>
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>
#include <vector>
#include "server_thread.h"
#include "common_socket.h"
#include "server_index.h"
//#include "compare_bf.h"



class Acceptor : public Thread { 
    private:
        Socket& skt;
        Index& index;
        Key key;
        bool keep_talking;

    public:
        Acceptor(Socket& _skt, Index& index, Key key);
        virtual void run() override;
        virtual void stop() override;
        virtual bool isDead() override;
};



#endif
