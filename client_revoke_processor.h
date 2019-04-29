#ifndef CLIENT_REVOKE_PROCESSOR_H
#define CLIENT_REVOKE_PROCESSOR_H
#include "common_socket.h"

class RevokeProcessor {
    private:
        Socket& skt;
    public:
        RevokeProcessor(Socket& skt);
        ~RevokeProcessor();
        void run(std::string certificate_filename, \
             std::string client_key_filname, std::string server_key_filename);
};

#endif
