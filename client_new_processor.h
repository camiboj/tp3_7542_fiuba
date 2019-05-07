#ifndef CLIENT_NEW_PROCESSOR_H
#define CLIENT_NEW_PROCESSOR_H
#include <string>
#include "common_protocol.h"

class NewProcessor {
    private:
        Protocol& skt;
    public:
        explicit NewProcessor(Protocol& skt);
        ~NewProcessor();
        void run(std::string& certificate_information_filename, \
                        std::string& client_key_filename, \
                        std::string& server_key_filename);
};

#endif
