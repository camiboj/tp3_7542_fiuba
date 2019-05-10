#include <string>
#include <iostream>
#include "client_revoke_processor.h"
#include "client_invalid_request.h"
#include "common_certificate.h"
#include "common_rsa.h"
#define HASH_ERROR_SM 2
#define HASH_ERROR_MSSG "Error: los hashes no coinciden.\n"
#define INVALID_CERTIFICATE 1
#define INVALID_CERTIFICATE_MSSG "Error: usuario no registrado.\n"
#define HASH "Hash calculado: "
#define PRIVATE_ENCRYPTION "Hash encriptado con la clave privada: "
#define PUBLIC_ENCRYPTION "Huella enviada: "

RevokeProcessor::RevokeProcessor(Protocol& _protocol) : Processor(_protocol) {}
RevokeProcessor::~RevokeProcessor() {}

void RevokeProcessor::run(std::string& certificate_filename,\
         std::string& client_key_filename, std::string& server_key_filename) {
    uint8_t command = 1;
    try {
        this->protocol.send(command);
    }
    catch (std::runtime_error) {
        throw std::runtime_error("Error while sending command new");
    }
    
    Certificate certificate;
    Rsa rsa;
    try {
        Key server_key(server_key_filename);
        Key client_key(client_key_filename);
        rsa.set(server_key, client_key);
    }
    catch(...) {
        throw std::runtime_error("Error creating key");
    }
    uint32_t hash;

    try {
        hash = certificate.send(certificate_filename, protocol);
    }
    catch(...) {
        __throw_exception_again;
    }

    uint32_t priv_encryption = rsa.privateEncryption(hash);
    uint32_t publ_encryption = rsa.publicEncryption(priv_encryption);
    try {
        this->protocol.send(publ_encryption);
    }
    catch (std::runtime_error) {
        throw std::runtime_error("Error while sending encrypted hash");
    }
    uint8_t status = 0;
    this->protocol.receive(status);
    
    std::cout << HASH << (int) hash << '\n';
    std::cout << PRIVATE_ENCRYPTION << (int) priv_encryption << '\n';
    std::cout << PUBLIC_ENCRYPTION << (int) publ_encryption << '\n';
    if (status == HASH_ERROR_SM) {
        throw InvalidRequest(HASH_ERROR_MSSG);
    }
    if (status == INVALID_CERTIFICATE) {
        throw InvalidRequest(INVALID_CERTIFICATE_MSSG);
    }
    return;
}
