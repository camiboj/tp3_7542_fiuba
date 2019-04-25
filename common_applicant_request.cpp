#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "common_certificate_info_parcer.h"
#include "common_applicant_request.h"
#include "common_time.h"
#define COMMAND_SIZE 1


ApplicantRequest::ApplicantRequest(std::string& cert_filename,\
                                 std::string& key_filename) {
        this->key.set(key_filename);
        CertificateInfoParcer cir(cert_filename, this->subject, \
                                  this->date_from, this->date_to);
}
ApplicantRequest::ApplicantRequest() {}
ApplicantRequest::~ApplicantRequest() {}

void ApplicantRequest::send(Socket& skt) {
    uint8_t command = 0;
    String sub_aux(this->subject);
    String from(this->date_from);
    String to(this->date_to);

    skt.sendAll(&command, COMMAND_SIZE);
    sub_aux.send(skt);
    key.send(skt);
    from.send(skt);
    to.send(skt);
}

void ApplicantRequest::recive(Socket& skt) {
        String my_subject(this->subject);
        my_subject.recive(skt);
        std::cerr << "Subject: " << this->subject << '\n';

        Key clien_public_key;
        clien_public_key.recive(skt);
        
        String my_date_to(this->date_from);
        my_date_to.recive(skt);
        std::cerr << "To: " << this->date_from << '\n';

        String my_date_from(this->date_to);
        my_date_from.recive(skt);
        std::cerr << "From: " << this->date_to << '\n';
}