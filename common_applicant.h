#ifndef COMMON_APPLICANT_H
#define COMMON_APPLICANT_H
#include <stdint.h>
#include <string>
#include "common_socket.h"
#include "common_key.h"
#include "common_string.h"

/*  - Comando:     - 1 byte sin signo (0)
 *
 *  - Subject:     - Longitud: 4 bytes big endian sin signo.
 *                 - String sin ‘\0’
 *
 *  - Módulo:      - 2 bytes en big endian sin signo
 * 
 *  - Exponente:   - 1 byte
 *
 *  - Fechas:      - Longitud: 4 bytes big endian sin signo.
 *                 - String sin ‘\0’
 * */

class Applicant {
    private:
        std::string subject;
        Key key;
        std::string date_from;
        std::string date_to;

    public:
        Applicant(std::string& filename, Key _key);
        ~Applicant();
        void send (Socket skt);
};

#endif