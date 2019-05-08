#include <iostream>
#include <string>
#include "server_revoke_client_processor.h"
#include "common_certificate.h"
#include "common_rsa.h"
#include "common_hash.h"
#define HASH_ERROR_MSSG 2
#define HASH_ERROR 1
#define INVALID_CERTIFICATE_MSSG 1
#define INVALID_CERTIFICATE 1
#define OK 0
#define OK_MSSG 0



RevokeClientProcessor::RevokeClientProcessor(Protocol& _skt,\
                                             Index& _index, Key& _key):
    index(_index), 
    server_key(_key),
    is_dead(false) {
        skt = std::move(_skt);
    }

RevokeClientProcessor::~RevokeClientProcessor() {}

void RevokeClientProcessor::run() {
    Certificate certificate;
    certificate.receive(skt);
    uint32_t encryption = 0;
    this->skt.receive(encryption);
    uint8_t answer;
    // race condition ->
    if (!this->index.hasCertificate(certificate)) { //ACA!
        answer = INVALID_CERTIFICATE_MSSG;
        this->skt.send(answer);
        return;
    }
    Key client_key = index.findCertificate(certificate);
    Rsa rsa(client_key, server_key);
    uint32_t desencryption = rsa.privateDesencryption(encryption);
    uint32_t client_hash = rsa.publicDesencryption(desencryption);
    std::string formal_certificate = certificate.toString();
    Hash hash(formal_certificate);
    uint32_t my_hash = hash();
    if (my_hash != client_hash) {
        answer = HASH_ERROR_MSSG;
        this->skt.send(answer);
        return;
    }
    index.eraseCertificate(certificate);
    answer = OK_MSSG;
    this->skt.send(answer);
    is_dead = true;
    return;
}

bool RevokeClientProcessor::isDead() {
   return is_dead;
}


void RevokeClientProcessor::stop() {
    this->skt.stop();
}
