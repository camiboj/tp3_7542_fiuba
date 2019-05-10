#ifndef CLIENT_NEW_PROCESSOR_H
#define CLIENT_NEW_PROCESSOR_H
#include <string>
#include "common_protocol.h"
#include "client_processor.h"

class NewProcessor : Processor{
    private:
//        Protocol& protocol;
    public:
        explicit NewProcessor(Protocol& protocol);
        ~NewProcessor();
        virtual void run(std::string& certificate_information_filename, \
                        std::string& client_key_filename, \
                        std::string& server_key_filename) override;
};

#endif
