#ifndef CLIENT_REVOKE_PROCESSOR_H
#define CLIENT_REVOKE_PROCESSOR_H
#include "common_protocol.h"
#include "client_processor.h"
#include <string>

class RevokeProcessor : Processor{
    private:
        //Protocol& protocol;

    public:
        explicit RevokeProcessor(Protocol& _protocol);
        ~RevokeProcessor();
        virtual void run(std::string& certificate_filename, \
             std::string& client_key_filename, \
             std::string& server_key_filename) override;
};

#endif
