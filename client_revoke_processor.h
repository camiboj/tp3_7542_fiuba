#ifndef CLIENT_REVOKE_PROCESSOR_H
#define CLIENT_REVOKE_PROCESSOR_H
#include "common_my_socket.h"
#include <string>

class RevokeProcessor {
    private:
        MySocket& skt;

    public:
        explicit RevokeProcessor(MySocket& skt);
        ~RevokeProcessor();
        void run(std::string certificate_filename, \
             std::string client_key_filname, std::string server_key_filename);
};

#endif
