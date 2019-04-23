#ifndef COMMON_KEY_H
#define COMMON_KEY_H
#include <stdint.h>
#include <string>
#include "common_socket.h"

class Key {
    protected:
        uint8_t exponent;   //1 byte
        uint16_t module;    //2 bytes
    public:
        Key(std::string filename);
        Key(uint8_t _exponent, uint16_t _module);
        ~Key();
        void send(Socket skt);
};

#endif