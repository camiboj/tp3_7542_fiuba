#include "common_string.h"
#include <endian.h>
#define LEN_SIZE 4

String::String(std::string& _str): str(_str) {
    len = htobe16(str.length());
}

void String::send(Socket skt) {
    skt.send_all(&len, LEN_SIZE);
    skt.send_all(this->str, str.length());
    //no mandar el /0 !!!
    //igual, si itero hasta length no deberia mandarlo
}
