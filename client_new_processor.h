#ifndef CLIENT_NEW_PROCESSOR_H
#define CLIENT_NEW_PROCESSOR_H
#include <string>
#include "common_my_socket.h"

class NewProcessor {
    private:
        MySocket& skt;
    public:
        NewProcessor(MySocket& skt);
        ~NewProcessor();
        void run(std::string client_key_filename, \
                    std::string certificate_information_filename);
};

#endif
