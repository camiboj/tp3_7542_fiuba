#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "client_applicant_request.h"
#include "client_certificate_info_parser.h"
#define COMMAND_SIZE 1


ApplicantRequest::ApplicantRequest(std::string& cert_filename,\
                                 std::string& key_filename) {
        CertificateInfoParser cir;
        try {
            this->key.set(key_filename);
            cir.run(cert_filename, this->subject, \
                       this->date_from, this->date_to);
        }
        catch(...) {
            throw std::runtime_error("Error creating aplicant request");
        }
}

ApplicantRequest::~ApplicantRequest() {}

void ApplicantRequest::send(Protocol& protocol) {
    uint8_t command = 0;
    try {
        protocol.send(command);
        protocol.send(this->subject);
        key.send(protocol);
        protocol.send(this->date_from);
        protocol.send(this->date_to);
    }
    catch (std::runtime_error) {
        throw std::runtime_error("Error while sending aplicant request");
    }
}

std::string ApplicantRequest::getSubject(){
    return this->subject;
}

Key ApplicantRequest::getClientKey() {
    return this->key;
}
