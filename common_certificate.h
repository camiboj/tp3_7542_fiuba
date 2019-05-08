#ifndef COMMON_CERTIFICATE_H
#define COMMON_CERTIFICATE_H
#include <stdint.h>
#include <string>
#include <queue>
#include <istream>
#include "common_key.h"
#include "common_protocol.h"

class Certificate {
    private:
        uint32_t serial_number;
        std::string subject;
        std::string issuer;
        std::string not_before;
        std::string not_after;
        Key key;

    public:
        Certificate(std::string& _subject, std::string& _not_before,\
                     std::string& _not_after, Key _key);
        Certificate();
        ~Certificate();
        /*
        * Se envian los datos del certificado por el socket.
        */
        void send(Protocol& skt);
        /*
        * Se lee el archivo contenedor del certificado,
        * se envian los datos a traves del socket
        * y se devuelve el valor del certificado hasheado
        */
        uint32_t send(std::string& filename, Protocol& skt);
        /*
        * Se reciben los datos del certificado por el socket.
        */
        void receive(Protocol& skt);
        /*
         * Seter del serial number
        */
        void addSerial(uint32_t serial_number);
        /*
         * Se imprime el certificado con formato:
         *
         * certificate:
         *    serial number: n (hexa n 4 bytes)
         *    subject: <subject>
         *    issuer: Taller de programacion 1
         *    validity:
         *        not before: <MMM DD HH:mm:SS YYYY>
         *        not after: <MMM DD HH:mm:SS YYYY>
         *    subject public key info:
         *        modulus: n (hexa n 2 bytes)
         *        exponent: 19 (hexa n 1 byte)
        */  
        std::string toString();
        std::string getSubject();
        Key getKey();
        friend std::ostream& operator<<(std::ostream &o, Certificate& self);
        //friend std::istream& operator>> (std::istream &in, Certificate& self);
};

#endif
