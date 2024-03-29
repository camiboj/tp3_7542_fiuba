#include <vector>
#include "server_acceptor.h"
#include "common_protocol.h"
#include "server_new_client_processor.h"
#include "server_revoke_client_processor.h"
#include "common_key.h"
#include "server_client_processor.h"

Acceptor::Acceptor(Socket& _skt, Index& _index, Key& _key):
    index(_index), 
    key(_key),
    keep_talking(true) {
        this->skt = std::move(_skt);
    }
        
void Acceptor::run() {
try {
    while (this->keep_talking) {
        Socket client_skt;
        try {
            client_skt = this->skt.acceptClient();
        }
        catch (std::runtime_error &e) {
            std::runtime_error(e.what());
        }

        Protocol protocol(client_skt);

        uint8_t command;
        protocol.receive(command);
        
        ClientProcessor* client;
        if (command == 0) {
            client = new New(protocol, index, key);
        }
        if (command == 1) {
            client = new RevokeClientProcessor(protocol, index, key);
        }
        this->clients.push_back(client);
        client->start();
        std::vector<ClientProcessor*>::iterator it = this->clients.begin();
        while (it != this->clients.end()) {
	        ClientProcessor* client = *it;
            if (client->isDead()) {
                client->join(); 
	            delete client;
	            this->clients.erase(it);
	        } else {
                ++it;
            }
        }
    }
}
catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
}
}

void Acceptor::stop() {
    this->keep_talking = false;
    skt.kill();
}

bool Acceptor::isDead() {
    return !keep_talking;
}

Acceptor::~Acceptor() {
    std::vector<ClientProcessor*>::iterator it = this->clients.begin();
    for (; it != this->clients.end(); ++it) {
	    ClientProcessor* client = *it;
            client->join(); 
	    	delete client;
    }
}
