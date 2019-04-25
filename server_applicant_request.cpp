#include <string>
#include <iostream>
#include "server_applicant_request.h"


ApplicantRequest::ApplicantRequest(Index& _index) : index(_index){
}

ApplicantRequest::~ApplicantRequest() {}

int ApplicantRequest::recive(Socket& skt) {
        String my_subject(this->subject);
        my_subject.recive(skt);
        //std::cerr << "Subject: " << this->subject << '\n';

        Key clien_public_key;
        clien_public_key.recive(skt);
        
        std::cerr << "Recien creada, por fuera\n" << clien_public_key << '\n';

        String my_date_to(this->date_from);
        my_date_to.recive(skt);
        //std::cerr << "To: " << this->date_from << '\n';

        String my_date_from(this->date_to);
        my_date_from.recive(skt);
        //std::cerr << "From: " << this->date_to << '\n';    
        uint8_t answer = 1;
        if (index.hasCertificate(this->subject)) {
            answer = 0;
            skt.sendAll(&answer, 1);
            //trow algo;
            return answer;
        } 
        skt.sendAll(&answer, 1);
        std::cerr << "Antes de crear new certificate: \n" << clien_public_key << '\n';
        this->index.newCertificate(subject, clien_public_key);
        return answer;
}