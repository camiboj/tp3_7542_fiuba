#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include "common_socket.h"
#include "client_new_processor.h"
#include "client_revoke_processor.h"
#include "client_invalid_request.h"
#define ARGUMENT_ERROR_MSSG "Error: argumentos invalidos.\n"
#define CF_SIZE 4 //certificate footprint size in bytes
#define HASH_SIZE 4


int main(int argc, char* argv[]) {
try {    
    if (argc != 7) {
        std::cout << ARGUMENT_ERROR_MSSG;
        return 0;
    }

    std::string mode1("new");
    std::string mode2("revoke");
    std::string mode = std::string(argv[3]);
    if (mode != mode1 && mode != mode2) {
        std::cout << ARGUMENT_ERROR_MSSG; //no es un throw 
                                          //porque no hay donde catchearlo
        return 0;
    }

    char* host = argv[1];
    char* port = argv[2];
    Socket skt;
    skt.connectWithServer(host, port);
    
    Protocol protocol(skt);

    
    std::string client_key_filename = std::string(argv[5]);
    std::string server_key_filename = std::string(argv[6]);
    
    if (mode == mode1) {
        std::string certificate_information_filename(argv[4]);
        NewProcessor np(protocol);
        np.run(certificate_information_filename, \
                    client_key_filename, \
                    server_key_filename);
        return 0;
    }

    if (mode == mode2) { //std::string(argv[4])
        std::string certificate_filename = std::string(argv[4]);
        RevokeProcessor rp(protocol);
        rp.run(certificate_filename, \
                    client_key_filename, \
                     server_key_filename);
        return 0;
    }
    return 0;
}
catch(InvalidRequest &e) {
    std::cout << e.what();
}
catch(std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
}
}
