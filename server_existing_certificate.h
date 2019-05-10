#ifndef SERVER_EXISTING_CERTIFICATE_H
#define SERVER_EXISTING_CERTIFICATE_H
#include <exception>
#include <string>

class ExistingCertificate: public std::exception {
protected:
        std::string msg;
    public:
        explicit ExistingCertificate(const char* message);

        explicit ExistingCertificate(const std::string& message);

        virtual ~ExistingCertificate() throw();

        virtual const char* what() const throw();
};

#endif
