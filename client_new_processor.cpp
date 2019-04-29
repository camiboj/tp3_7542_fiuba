#include <string>
#include <iostream>
#include <fstream>
#include "client_new_processor.h"
#include "client_applicant_request.h"
#include "common_socket.h"
#include "common_certificate.h"
#include "common_hash.h"
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


NewProcessor::NewProcessor(Socket& _skt) : 
skt(_skt) {}

NewProcessor::~NewProcessor() {}

void NewProcessor::run(std::string client_key_filename, \
                        std::string certificate_information_filename) {
    ApplicantRequest request(certificate_information_filename,\
                                client_key_filename);
    request.send(skt);
    uint8_t answer = 1;
    skt.reciveNumber(&answer);
    if (answer == CERIFICATE_ERROR_RECIVED_MSSG) {
        std::cout << CERTIFICATE_ERROR_MSSG;
        return;
    }

    Certificate certificate;
    certificate.recive(skt);
    std::string formal_certificate;
    formal_certificate = certificate.toString();


    Hash hash(formal_certificate);
    uint32_t my_hash =  hash();


    uint32_t server_hash = 0;
    skt.reciveNumber(&server_hash);

    uint32_t certificate_footprint = 0;
    skt.reciveNumber(&certificate_footprint);

    uint8_t notification = HASH_OK_SERVER_MSSG;
    if (my_hash != server_hash) {
        notification = HASH_ERROR_SERVER_MSSG;
        skt.sendNumber(notification);
        std::cout << HASH_ERROR_MSSG;
        return;
    }
    skt.sendNumber(notification);
    std::cout << CERT_FP << certificate_footprint << '\n';
    std::cout << SH << server_hash << '\n';
    std::cout << MH << my_hash << '\n';

    std::string filename = request.getSubject() + ".cert";
    std::ofstream file;
    file.open(filename, std::ofstream::out | std::ofstream::trunc);
    
    if (!file.is_open()) {
        //exc
    }
    file << formal_certificate;
    return;
}