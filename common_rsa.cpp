#include "common_rsa.h"
//size in bytes of the message to encrypt
#define HASH_SIZE 4

Rsa::Rsa(Key& public_key, Key& private_key) {
    this->public_exponent = public_key.getPublicExponent();
    this->public_module = public_key.getModule();
    this->private_exponent = private_key.getPrivateExponent();
    this->private_module = private_key.getModule();
}

Rsa::Rsa(){}

void Rsa::set(Key& public_key, Key& private_key) {
    this->public_exponent = public_key.getPublicExponent();
    this->public_module = public_key.getModule();
    this->private_exponent = private_key.getPrivateExponent();
    this->private_module = private_key.getModule();
}

Rsa::~Rsa() {}

uint32_t encrypt(const uint32_t exponent, const uint32_t module,\
                 const uint32_t hash) {
    uint32_t result;
    uint32_t base;

    uint32_t ret = 0;

    for (uint32_t i = 0; i < HASH_SIZE; ++i) {
        result = (hash >> (i * 8)) & 0xff;
        base = result;
        for (uint32_t j = 1; j < exponent; ++j)
            result = (result * base) % module;
        ret = ret + (result << (i * 8));
    }
    return ret;
}


uint32_t Rsa::publicEncryption(const uint32_t hash) {
    return encrypt(this->public_exponent, this->public_module, hash);
}

uint32_t Rsa::privateEncryption(const uint32_t hash) {
    return encrypt(this->private_exponent, this->private_module, hash);
}

uint32_t Rsa::publicDesencryption(const uint32_t hash){
    return encrypt(this->public_exponent, this->public_module, hash);
}

uint32_t Rsa::privateDesencryption(const uint32_t hash) {
    return encrypt(this->private_exponent, this->private_module, hash);
}

