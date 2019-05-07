#include <string>
#include <iostream>
#include "server_index.h"
#include "server_acceptor.h"


#define ERROR_CODE 1
#define COMMAND_SIZE 1
#define LEN_SIZE 4


int main(int argc, char* argv[]) {
    if (argc != 4) {
        return ERROR_CODE;
    }

    std::string claves = std::string(argv[2]);
    std::string indice = std::string(argv[3]);
     if (argc !=4) {
	    return 1;
    }

    Socket skt;
    skt.connectWithClients(argv[1]);

    std::string index_filename(argv[3]);
    Index index(index_filename);
    
    
    //skt.acceptClient();
    std::string key_filename = std::string(argv[2]);
    Key key(key_filename);
    Acceptor acceptor(skt, index, key);
    acceptor.start();

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "q") {
            acceptor.stop();
            break;
        }
    }    
    acceptor.join();

    
    index.write();    
    return 0;
}
