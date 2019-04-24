#ifndef COMMON_STRING_H
#define COMMON_STRING_H
#include <stdint.h>
#include <string>
#include "common_socket.h"
#include "common_key.h"
/*
 * Longitud: 4 bytes big endian sin signo.
 * String:  sin ‘\0’
*/

class String {
    private:
        std::string& str;
        uint32_t len;
    public:
        String(std::string& _str);
        ~String();
        void send(Socket skt);
        void recive(Socket skt);
};

#endif