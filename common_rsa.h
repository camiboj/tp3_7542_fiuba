#ifndef RSA_H_
#define RSA_H_
#include <stdint.h>

/*
* Algoritmo de encriptación asimétrico.
*/
class Rsa {
    private:
        uint8_t exponent;
        uint16_t modulus;
    public:
        Rsa(uint8_t _exponent, uint16_t _modulus);
        ~Rsa();

        /*
        * Recibe un mensaje (que es un número de 4 bytes sin signo) y lo 
        * encrita en otro entero de 4 bytes sin signo.
        */
        uint32_t operator()(const uint32_t message);
};

#endif
