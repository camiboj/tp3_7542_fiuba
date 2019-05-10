#ifndef RSA_H
#define RSA_H
#include <stdint.h>
#include "common_key.h"

/*
* Algoritmo de encriptación asimétrico.
*/
class Rsa {
    private:
        uint8_t public_exponent;
        uint16_t public_module;
        uint8_t private_exponent;
        uint16_t private_module;
    public:
        Rsa(Key& public_key, Key& private_key);
        Rsa();
        ~Rsa();

        void set(Key& public_key, Key& private_key);

        /*
        * Recibe un mensaje (que es un número de 4 bytes sin signo) y lo 
        * encrita en otro entero de 4 bytes sin signo.
        */
        uint32_t publicEncryption(const uint32_t hash);
        uint32_t privateEncryption(const uint32_t hash);
        uint32_t publicDesencryption(const uint32_t hash);
        uint32_t privateDesencryption(const uint32_t hash);
};

#endif
