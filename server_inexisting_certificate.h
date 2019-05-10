#ifndef SERVER_INEXISTING_CERTIFICATE_H
#define SERVER_INEXISTING_CERTIFICATE_H
#include <exception>
#include <string>

class InexistingCertificate: public std::exception {
protected:
        std::string msg;
    public:
        explicit InexistingCertificate(const char* message);

        explicit InexistingCertificate(const std::string& message);

        virtual ~InexistingCertificate() throw();

        virtual const char* what() const throw();
};

#endif
