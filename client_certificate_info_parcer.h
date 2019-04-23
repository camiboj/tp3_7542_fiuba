#ifndef CLIENT_CERTIFICATE_INFO_PARCER_H
#define CLIENT_CERTIFICATE_INFO_PARCER_H
#include <string>
#include "common_socket.h"

/******************************************************************************
 * Lee una rchivo de tipo 
 *   <subject>\n<date from>\n<date to>
 * Donde las fechas son de tipo MMM DD HH:mm:SS YYYY
 * Y almacena los atributos leidos en los repectivos string pasados por parame-
 * tro.
 * Si las fechas no existen from sera la fecha actual y to 30 dias despues.
*/

class CertificateInfoParcer {
    private:
        
    public:
        CertificateInfoParcer(std::string filename, std::string subject,\
                             std::string from, std::string to);
        ~CertificateInfoParcer();
};

#endif