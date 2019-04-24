#ifndef COMMON_KEY_H
#define COMMON_KEY_H
#include <stdint.h>
#include <string>
#include "common_socket.h"

class Key {
    private:
        uint8_t public_exponent;   //1 byte
        uint8_t private_expponent; //1 byte
        uint16_t module;    //2 bytes
    public:
        Key();
        //Key(uint8_t _exponent, uint16_t _module);
        ~Key();
        /*
         * Recibe el nombre de un archivo de tipo
         * <exp_publico> <exp_privado> <modulo>
         *
         * Los distintos campos pueden estar separados por 1 o más espacios, 
         * únicamente se garantiza que estos se encuentran en una misma línea.
        */
        void set(std::string filename);
        /*
        * Envia, a traves del socket pasado por parametro:
        * Módulo: en formato 2 bytes en big endian sin signo.
        * Exponente: en 1 byte.
        */
        void send(Socket skt);
        void recive(Socket skt);
};

#endif