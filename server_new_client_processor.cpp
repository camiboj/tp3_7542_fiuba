#include <string>
#include <iostream>
#include "server_new_client_processor.h"
#include "server_inexisting_certificate.h"
#include "server_existing_certificate.h"
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

New::New(Protocol& _protocol, \
                                        Index& _index, Key& _key) :
    index(_index), 
    server_key(_key),
    is_dead(false) {
        this->protocol = std::move(_protocol);
    }

New::~New() {}

void New::receiveInfo() {
    protocol.receive(this->subject);
    this->client_key.receive(protocol);
    
    protocol.receive(this->date_from);

    protocol.receive(this->date_to);
    if (subject.size() == 0) {
        throw std::runtime_error("");
    }
}

std::string New::createCertificate() {
    std::string subj = this->subject;
    Certificate certificate(subj, this->date_from, this->date_to,\
             this->client_key);

    uint8_t answer = CERTIFICATE_OK;
    try {
        this->index.save(certificate);
    }
    catch(ExistingCertificate) { //ExistingCertificate
        answer = CERTIFICATE_ERROR;
        try {
            protocol.send(answer);
        }
        catch(std::runtime_error) {
            throw std::runtime_error(
     "Error, client could not be notified that there was a certificate error");
        }
        __throw_exception_again;
    }
    try {
        protocol.send(answer);
    }
    catch(std::runtime_error) {
        throw std::runtime_error(\
        "Error, the client could not be notified that there was no error");
    }

    std::string result = certificate.toString();
    try {
        certificate.send(protocol);
    }
    catch(std::runtime_error) {
        throw std::runtime_error(\
        "Error sending the certificate while processing new client");
    }
    return result;
}


uint32_t encrypt(Key client_key, Key server_key, uint32_t hash) {
    Rsa rsa(client_key, server_key);
    uint32_t encryption = rsa.privateEncryption(hash);
    encryption = rsa.publicEncryption(encryption);
    return encryption;
}

void New::run() {
    try {
        this->receiveInfo();
    }
    catch (...) {
        this->is_dead = true;
        return;
    }


    std::string formal_certificate;
    try {
        formal_certificate = this->createCertificate();
    } catch(ExistingCertificate) {
        return;
    }
    
    
    Hash hash(formal_certificate);
    uint32_t hashed_certificate =  hash();
    uint32_t encryption = encrypt(this->client_key, this->server_key,\
                                 hashed_certificate);
    try {
        protocol.send(encryption);
    }
    catch(std::runtime_error) {
        throw std::runtime_error(\
        "Error sending encrypted hash while processing new client");
    }
    uint8_t hash_status = 0;
    protocol.receive(hash_status);
    if (hash_status == HASH_ERROR) {
        try {
            index.erase(this->subject);
        }
        catch(InexistingCertificate){
            throw std::runtime_error("Error erasing certificate");
        }
    }
    this->is_dead = true;
    return;
}

bool New::isDead() {
    return this->is_dead;
}

void New::stop() {
    this->protocol.stop();
}
