#include <string>
#include <iostream>
#include "client_revoke_processor.h"
#include "common_certificate.h"
#include "common_rsa.h"
#define HASH_ERROR_SM 2
#define HASH_ERROR_MSSG "Error: los hashes no coinciden.\n"
#define INVALID_CERTIFICATE 1
#define INVALID_CERTIFICATE_MSSG "Error: usuario no registrado.\n"
#define HASH "Hash calculado: "
#define PRIVATE_ENCRYPTION "Hash encriptado con la clave privada: "
#define PUBLIC_ENCRYPTION "Huella enviada: "

RevokeProcessor::RevokeProcessor(MySocket& _skt) : skt(_skt) {}
RevokeProcessor::~RevokeProcessor() {}

void RevokeProcessor::run(std::string certificate_filename,\
         std::string client_key_filname, std::string server_key_filename) {
    uint8_t command = 1;
    this->skt.sendNumber(&command);
    Certificate certificate;
    uint32_t hash = certificate.send(certificate_filename, skt);
    Key server_key(server_key_filename);
    Key client_key(client_key_filname);
    Rsa rsa(server_key, client_key);
    uint32_t priv_encryption = rsa.privateEncryption(hash);
    uint32_t publ_encryption = rsa.publicEncryption(priv_encryption);
    this->skt.sendNumber(&publ_encryption);
    uint8_t status = 0;
    this->skt.reciveNumber(&status);
    if (status == HASH_ERROR_SM) {
        std::cout << HASH_ERROR_MSSG;
        return;
    }
    if (status == INVALID_CERTIFICATE) {
        std::cout << INVALID_CERTIFICATE_MSSG;
        return;
    }
    std::cout << HASH << (int) hash << '\n';
    std::cout << PRIVATE_ENCRYPTION << (int) priv_encryption << '\n';
    std::cout << PUBLIC_ENCRYPTION << (int) publ_encryption << '\n';
    return;
}