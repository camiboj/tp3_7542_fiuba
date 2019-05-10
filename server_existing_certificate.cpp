#include <string>
#include "server_existing_certificate.h"

ExistingCertificate::ExistingCertificate(const char* message):
      msg(message) {}

ExistingCertificate::ExistingCertificate(const std::string& message):
      msg(message) {}

ExistingCertificate::~ExistingCertificate() throw(){}

const char* ExistingCertificate::what() const throw(){
    return msg.c_str();
}
