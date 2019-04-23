#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "common_applicant_request.h"
#include "common_time.h"
#include "client_certificate_info_parcer.h"
#define COMMAND_SIZE 1


ApplicantRequest::ApplicantRequest(std::string& cert_filename, std::string& key_filename) {
        this->key.set(key_filename);
        CertificateInfoParcer cir(cert_filename, this->subject, \
                                  this->date_from, this->date_to);
}

ApplicantRequest::~ApplicantRequest(){}

void ApplicantRequest::send(Socket skt) {
    uint8_t command = 0;
    String sub_aux(this->subject);
    String from(this->date_from);
    String to(this->date_to);

    skt.send_all(&command, COMMAND_SIZE);
    sub_aux.send(skt);
    key.send(skt);
    from.send(skt);
    to.send(skt);
}