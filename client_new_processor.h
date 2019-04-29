#ifndef CLIENT_NEW_PROCESSOR_H
#define CLIENT_NEW_PROCESSOR_H
#include <string>
#include "common_socket.h"

class NewProcessor {
    private:
        Socket& skt;
    public:
        NewProcessor(Socket& skt);
        ~NewProcessor();
        void run(std::string client_key_filename, \
                    std::string certificate_information_filename);
};

#endif
