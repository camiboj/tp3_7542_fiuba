#include <string>
#include <iostream>
#include "common_hash.h"
#define INITIAL_COUNT 0

Hash::Hash() {
    this->count = INITIAL_COUNT;
}
Hash::~Hash() {}

Hash::Hash(std::string str) {
    this->count = INITIAL_COUNT;
    std::string::iterator it = str.begin();
    for (; it!=str.end(); ++it) {
        this->count += (uint8_t)*it;
    }
}

uint32_t Hash::operator()() {
    return this->count;
}
