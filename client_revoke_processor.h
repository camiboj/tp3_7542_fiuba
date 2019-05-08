#ifndef CLIENT_REVOKE_PROCESSOR_H
#define CLIENT_REVOKE_PROCESSOR_H
#include "common_protocol.h"
#include <string>

class RevokeProcessor {
    private:
        Protocol& skt;

    public:
        explicit RevokeProcessor(Protocol& skt);
        ~RevokeProcessor();
        void run(std::string& certificate_filename, \
             std::string& client_key_filename, \
             std::string& server_key_filename);
};

#endif
