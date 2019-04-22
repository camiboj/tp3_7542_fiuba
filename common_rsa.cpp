#include "common_rsa.h"
//size in bytes of the message to encrypt
#define SIZE_MESSAGE 4

Rsa::Rsa(uint8_t _exponent, uint16_t _modulus):
    exponent(_exponent), modulus(_modulus) {}

Rsa::~Rsa() {}

uint32_t Rsa::operator()(const uint32_t message) {
    uint32_t result;
    uint32_t base;

    uint32_t ret = 0;

    for (int i = 0; i < SIZE_MESSAGE; ++i) {
        result = (message >> (i * 8)) & 0xff;
        base = result;
        for (int j = 1; j < this->exponent; ++j)
            result = (result * base) % this->modulus;
        ret = ret + (result << (i * 8));
    }
    return ret;
}
