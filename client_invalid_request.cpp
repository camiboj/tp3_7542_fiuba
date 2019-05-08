#include "client_invalid_request.h"
#include <string>

InvalidRequest::InvalidRequest(const char* message):
      msg(message) {}

InvalidRequest::InvalidRequest(const std::string& message):
      msg(message) {}

InvalidRequest::~InvalidRequest() throw(){}

const char* InvalidRequest::what() const throw(){
    return msg.c_str();
}
