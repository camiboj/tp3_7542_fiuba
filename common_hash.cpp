#include "common_hash.h"
#include <string>

Hash::Hash() {}
Hash::~Hash() {}

void Hash::load(std::string str) {
    std::string::iterator it=str.begin();
    for (; it!=str.end(); ++it) {
        this->count += (uint8_t)*it;
    }
}

uint16_t Hash::operator()() {
    return this->count;
}
