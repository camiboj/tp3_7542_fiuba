#include <iostream>
#include <string>
#include "server_revoke_client_processor.h"
#include "server_inexisting_certificate.h"
#include "common_certificate.h"
#include "common_rsa.h"
#include "common_hash.h"
#define HASH_ERROR_MSSG 2
#define HASH_ERROR 1
#define INVALID_CERTIFICATE_MSSG 1
#define INVALID_CERTIFICATE 1
#define OK 0
#define OK_MSSG 0



RevokeClientProcessor::RevokeClientProcessor(Protocol& _protocol,\
                                             Index& _index, Key& _key):
    index(_index), 
    server_key(_key),
    is_dead(false) {
        protocol = std::move(_protocol);
    }

RevokeClientProcessor::~RevokeClientProcessor() {}

void RevokeClientProcessor::run() {
    Certificate certificate;
    certificate.receive(protocol);
    uint32_t encryption = 0;
    this->protocol.receive(encryption);
    uint8_t answer;
    Key client_key = index.find(certificate);

    try {
        this->index.erase(certificate);
    }
    catch(InexistingCertificate) {
        answer = INVALID_CERTIFICATE_MSSG;
        try {
            this->protocol.send(answer);
        }
        catch(std::runtime_error) {
            throw std::runtime_error(\
    "Error, client could not be notified that there was a certificate error");
        }
        return;
    }
    Rsa rsa(client_key, server_key);
    uint32_t desencryption = rsa.privateDesencryption(encryption);
    uint32_t client_hash = rsa.publicDesencryption(desencryption);
    std::string formal_certificate = certificate.toString();
    Hash hash(formal_certificate);
    uint32_t my_hash = hash();
    if (my_hash != client_hash) {
        //std::cerr << "client hash: " << client_hash << '\n';
        //std::cerr << "my hash: " << my_hash << '\n';
        answer = HASH_ERROR_MSSG;
        this->index.putBack(certificate, client_key);
        try {
            this->index.putBack(certificate, client_key);
            this->protocol.send(answer);
        }
        catch(std::runtime_error) {
            throw std::runtime_error(\
      "Error, the client could not be notified  that there was a hash errort");
        }
        catch(...) {
            __throw_exception_again;
        }
        return;
    }
    //index.erase(certificate);
    answer = OK_MSSG;
    try {
        this->protocol.send(answer);
    }
    catch(std::runtime_error) {
        throw std::runtime_error(\
            "Error, the client could not be notified that there was no error");
    }
    is_dead = true;
    return;
}

bool RevokeClientProcessor::isDead() {
   return is_dead;
}


void RevokeClientProcessor::stop() {
    this->protocol.stop();
}
