#include <string>
#include <iostream>
#include <fstream>
#include "client_new_processor.h"
#include "client_applicant_request.h"
#include "common_socket.h"
#include "common_certificate.h"
#include "common_hash.h"
#include "common_rsa.h"
#include "client_invalid_request.h"
#define USER_ERROR_MSSG "Error: usuario no registrado.\n"
#define USER_ERROR 0
#define CERTIFICATE_ERROR_MSSG "Error: ya existe un certificado.\n"

#define CERIFICATE_ERROR_RECIVED_MSSG 0
#define HASH_ERROR_MSSG "Error: los hashes no coinciden.\n"
#define HASH_ERROR_SERVER_MSSG 1
#define HASH_OK_SERVER_MSSG 0
#define CERT_FP "Huella del servidor: "
#define SH "Hash del servidor: "
#define MH "Hash calculado: "


NewProcessor::NewProcessor(Protocol& _protocol) : Processor(_protocol) {}

NewProcessor::~NewProcessor() {}

void NewProcessor::run(std::string& certificate_information_filename, \
                        std::string& client_key_filename, \
                        std::string& server_key_filename) {
    ApplicantRequest request(certificate_information_filename,\
                                client_key_filename);
    try {
        request.send(protocol);
    }
    catch (std::runtime_error) {
        __throw_exception_again;
    }
    uint8_t answer = 1;
    protocol.receive(answer);
    if (answer == CERIFICATE_ERROR_RECIVED_MSSG) {
        throw InvalidRequest(CERTIFICATE_ERROR_MSSG);
    }

    Certificate certificate;
    certificate.receive(protocol);
    std::string formal_certificate;
    formal_certificate = certificate.toString();


    Hash hash(formal_certificate);
    uint32_t my_hash =  hash();


    uint32_t certificate_footprint = 0;
    protocol.receive(certificate_footprint);

    Key client_key = request.getClientKey();
    Key server_key;
    try {
        server_key.set(server_key_filename);
    }
    catch(std::runtime_error) {
        throw std::runtime_error("Error seting server key");
    }
    Rsa rsa(server_key, client_key);
    uint32_t pd =  rsa.privateDesencryption(certificate_footprint);
    uint32_t server_hash = rsa.publicDesencryption(pd);

    std::cout << CERT_FP << certificate_footprint << '\n';
    std::cout << SH << server_hash << '\n';
    std::cout << MH << my_hash << '\n';

    uint8_t notification = HASH_OK_SERVER_MSSG;
    if (my_hash != server_hash) {
        notification = HASH_ERROR_SERVER_MSSG;
        try {
            protocol.send(notification);
        }
        catch(std::runtime_error) {
            throw std::runtime_error(\
            "Error , the server could not be notified there was a hash error");
        }
        
        throw InvalidRequest(HASH_ERROR_MSSG);
    }

    try {
        protocol.send(notification);
    }
    catch(std::runtime_error) {
        throw std::runtime_error(\
        "Error , the server could not be notified there was no hash error");
    }

    std::string filename = request.getSubject() + ".cert";
    std::ofstream file;
    file.open(filename, std::ofstream::out | std::ofstream::trunc);
    
    if (!file.is_open()) {
        //exc
    }
    file << formal_certificate;
    return;
}
