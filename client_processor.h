#ifndef CLIENT_PROCESSOR_H_
#define CLIENT_PROCESSOR_H_
#include <string>
#include "common_protocol.h"

class Processor {
    protected:
        Protocol& protocol;
    public:
        explicit Processor(Protocol& _protocol);
        ~Processor();
        virtual void run(std::string& filename1, std::string& filename2, \
                        std::string& filename3) = 0; 
};

#endif
