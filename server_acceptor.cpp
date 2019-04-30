#include <vector>
#include "server_acceptor.h"
#include "common_my_socket.h"
#include "server_new_client_processor.h"
#include "server_revoke_client_processor.h"
#include "common_key.h"

Acceptor::Acceptor(Socket& _skt, Index& _index, Key _key): 
    skt(_skt), 
    index(_index), 
    key(_key),
    keep_talking(true) {}
        
void Acceptor::run() {
    std::vector<Thread*> clients;
    
    while (this->keep_talking) {
        this->client_skt = this->skt.acceptClient();
        MySocket my_socket(this->client_skt);

        uint8_t command;
        my_socket.receiveNumber(&command);
        
        Thread* client;
        if (command == 0) {
            client = new NewClientProcessor(my_socket, index, key);
        }
        if (command == 1) {
            client = new RevokeClientProcessor(my_socket, index, key);
        }
        clients.push_back(client);
        client->start();
        std::vector<Thread*>::iterator it = clients.begin();
        for (; it != clients.end(); ++it) {
	        Thread* client = *it;
            if (client->isDead()) {	
                //client->stop();		
                client->join(); 
	        	delete client;
	        	clients.erase(it);
	        }
	    }
    }
}

void Acceptor::stop() {
    this->keep_talking = false;
    skt.kill();
}

bool Acceptor::isDead() {
    return !keep_talking;
}
