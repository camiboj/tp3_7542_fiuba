#include <endian.h>
#include <iostream>
#include <string>
#include "common_string.h"
#define LEN_SIZE 4


String::String(std::string& _str): str(_str) {
    //std::cerr << "String: " << str << '\n';
    this->len = (uint32_t) str.length();
    //this->len = htobe32(aux);
}
void String::recive(Socket& skt) {
    uint32_t aux_len;
    skt.reciveSome(&aux_len, LEN_SIZE);
    this->len = htobe32(aux_len);

    skt.reciveSome(this->str, this->len);
}

void String::send(Socket& skt) {
    uint32_t aux = htobe32(this->len);
    skt.sendAll(&aux, LEN_SIZE);
    skt.sendAll(this->str, str.length());
    //no mandar el /0 !!!
    //igual, si itero hasta length no deberia mandarlo
}

String::~String() {}


/************************************ PRINT ***********************************
*    fprintf(stderr, "Len: %d \n", aux);
*    fprintf(stderr, "Len:");
*    fprintf(stderr, "Len: %d \n", this->len);
*
*    char* ptr = (char*) &aux;
*    uint8_t _aux = ptr[0];
*    fprintf(stderr, "Len: %d", _aux);
*    _aux = ptr[1];
*    fprintf(stderr, " %d", _aux);
*    _aux = ptr[2];
*    fprintf(stderr, " %d", _aux);
*    _aux = ptr[3];
*    fprintf(stderr, " %d", _aux);
*
*    std::cerr << "\nNow.. big endian: \n";
*
*    ptr = (char*) &this->len;
*    _aux = ptr[0];
*    fprintf(stderr, "Len: %d", _aux);
*    _aux = ptr[1];
*    fprintf(stderr, " %d", _aux);
*    _aux = ptr[2];
*    fprintf(stderr, " %d", _aux);
*    _aux = ptr[3];
*    fprintf(stderr, " %d", _aux);
*    std::cerr << '\n';
*    std::cerr << '\n';
*/
