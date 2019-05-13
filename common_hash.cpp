#include <string>
#include <iostream>
#include "common_hash.h"
#define INITIAL_COUNT 0

Hash::Hash() {
    this->count = INITIAL_COUNT;
}
Hash::~Hash() {}

Hash::Hash(std::string& str) {
    this->count = INITIAL_COUNT;
    this->load(str);
}

void Hash::load(std::string& str) {
    std::string::iterator it = str.begin();
    for (; it!=str.end(); ++it) {
        this->count += (uint8_t)*it;
    }
}

uint32_t Hash::operator()() {
    return this->count;
}
