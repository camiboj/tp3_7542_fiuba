#include "server_inexisting_certificate.h"
#include <string>

InexistingCertificate::InexistingCertificate(const char* message):
      msg(message) {}

InexistingCertificate::InexistingCertificate(const std::string& message):
      msg(message) {}

InexistingCertificate::~InexistingCertificate() throw(){}

const char* InexistingCertificate::what() const throw(){
    return msg.c_str();
}
