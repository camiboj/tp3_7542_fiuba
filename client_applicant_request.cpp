#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "client_applicant_request.h"
#include "client_certificate_info_parser.h"
#include "common_time.h"
#define COMMAND_SIZE 1


ApplicantRequest::ApplicantRequest(std::string& cert_filename,\
                                 std::string& key_filename) {
        this->key.set(key_filename);
        CertificateInfoParser cir(cert_filename, this->subject, \
                                  this->date_from, this->date_to);
}
ApplicantRequest::~ApplicantRequest() {}

void ApplicantRequest::send(Protocol& skt) {
    uint8_t command = 0;
    skt.send(command);
    skt.send(this->subject);
    key.send(skt);
    skt.send(this->date_from);
    skt.send(this->date_to);
}

std::string ApplicantRequest::getSubject(){
    return this->subject;
}

Key ApplicantRequest::getClientKey() {
    return this->key;
}
