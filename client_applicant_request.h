#ifndef CLIENT_APPLICANT_REQUEST_H
#define CLIENT_APPLICANT_REQUEST_H
#include <stdint.h>
#include <string>
#include "common_key.h"
#include "common_protocol.h"


class ApplicantRequest {
    private:
        std::string subject;
        Key key;
        std::string date_from;
        std::string date_to;

    public:
        /*
        * Recibe los dos archivos necesarios para solicitar un nuevo aplicante
        */
        ApplicantRequest(std::string& cert_filename, std::string& key_filename);
        
        ~ApplicantRequest();
        /*  
         * Envia a traves del socket recibido por parametro una solucitud con
         * formato:
         * 
         * <comando>        1 byte con valor 0.
         * <subject__size>  4 bytes big endian sin signo
         * <subject>        String sin ‘\0’
         * <modul>          2 bytes big endian sin signo
         * <exponent>       1 byte
         * <date__size>     4 bytes big endian sin signo
         * <date_from>      String sin ‘\0’
         * <date__size>     4 bytes big endian sin signo
         * <dat_to>         String sin ‘\0’
        */
        void send(Protocol& skt);
        std::string getSubject();
        Key getClientKey();
};

#endif
