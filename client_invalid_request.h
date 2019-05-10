#ifndef CLIENT_INVALID_REQUEST_H
#define CLIENT_INVALID_REQUEST_H
#include <exception>
#include <string>

class InvalidRequest: public std::exception {
protected:
        std::string msg;
    public:
        explicit InvalidRequest(const char* message);

        explicit InvalidRequest(const std::string& message);

        virtual ~InvalidRequest() throw();

        virtual const char* what() const throw();
};

#endif
