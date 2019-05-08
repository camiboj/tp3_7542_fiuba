#include <string>
#include <iostream>
#include "server_new_client_processor.h"
#include "common_certificate.h"
#include "common_hash.h"
#include "common_rsa.h"
#define ENCRYPTION_SIZE 4
#define HASH_ERROR 1
#define CERTIFICATE_ERROR 0
#define CERTIFICATE_OK 1
#define CERT_STATUS_SIZE 1
#define ERROR_CODE 1
#define HASH_STATUS_SIZE 1

NewClientProcessor::NewClientProcessor(Protocol& _skt, \
                                        Index& _index, Key& _key) :
    index(_index), 
    server_key(_key),
    is_dead(false) {
        this->skt = std::move(_skt);
    }

NewClientProcessor::~NewClientProcessor() {}

void NewClientProcessor::receiveInfo() {
    skt.receive(this->subject);
    this->client_key.receive(skt);
    
    skt.receive(this->date_from);

    skt.receive(this->date_to);
    if (subject.size() == 0) {
        throw std::runtime_error("");
    }
}

std::string NewClientProcessor::createCertificate() {
    std::string subj = this->subject;
    Certificate certificate(subj, this->date_from, this->date_to,\
             this->client_key);
    this->index.saveCertificate(certificate);
    std::string result = certificate.toString();
    certificate.send(skt);
    return result;
}

bool NewClientProcessor::checkCertificate() {
    uint8_t answer = CERTIFICATE_OK;
    if (index.hasCertificate(this->subject)) {
        answer = CERTIFICATE_ERROR;
        skt.send(answer);
        //trow algo;
        return false;
    } 
    skt.send(answer);
    return true;
}

uint32_t encrypt(Key client_key, Key server_key, uint32_t hash) {
    Rsa rsa(client_key, server_key);
    uint32_t encryption = rsa.privateEncryption(hash);
    encryption = rsa.publicEncryption(encryption);
    return encryption;
}

void NewClientProcessor::run() {
    try {
        this->receiveInfo();
    }
    catch (...) {
        this->is_dead = true;
        return;
    }
    bool s = this->checkCertificate();
    if (!s) return;
    std::string formal_certificate = this->createCertificate();
    
    Hash hash(formal_certificate);
    uint32_t hashed_certificate =  hash();
    uint32_t encryption = encrypt(this->client_key, this->server_key,\
                                 hashed_certificate);

    skt.send(encryption);
    
    uint8_t hash_status = 0;
    skt.receive(hash_status);
    if (hash_status == HASH_ERROR) {
        index.eraseCertificate(this->subject);
    }
    
    this->is_dead = true;
    return;
}

bool NewClientProcessor::isDead() {
    return this->is_dead;
}

void NewClientProcessor::stop() {
    this->skt.stop();
}
