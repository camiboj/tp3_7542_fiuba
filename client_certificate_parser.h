#ifndef CLIENT_CERTIFICATE_PARSER_H
#define CLIENT_CERTIFICATE_PARSER_H
#include <string>
#include "common_protocol.h"
#include "common_hash.h"

class CertificateParser {
    private:
        Hash hash;
    public:
        /*
        * Se lee el archivo contenedor del certificado,
        * se envian los datos a traves del socket
        * y se calcula el valor del certificado hasheado
        */
        CertificateParser(std::string& filename, Protocol& skt);
        uint32_t hashCertificate();
};

#endif