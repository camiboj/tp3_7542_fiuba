#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ios>
#include "common_string.h"
#include "common_certificate.h"
#include "common_hash.h"
#define CERTIFICATE "certificate:\n"
#define SERIAL_NUMBER "\tserial number: "
#define SN_SIZE 4
#define SUBJECT "\tsubject: "
#define ISSUER "\tissuer: Taller de programacion 1\n"
#define VALIDITY "\tvalidity:\n"
#define NOT_BEFOR "\t\tnot before: "
#define NOT_AFTER "\t\tnot after: "
#define KEY "\tsubject public key info:\n"
#define MODULE "\t\tmodulus: "
#define EXPONENT "\t\texponent: "
/******************************************************************************
 * certificate:
 *    serial number: 1 (0x00000001)
 *    subject: Federico Manuel Gomez Peter
 *    issuer: Taller de programacion 1
 *    validity:
 *        not before: Mar 28 21:33:04 2019
 *        not after: May 27 21:33:04 2019
 *    subject public key info:
 *        modulus: 253 (0x00fd)
 *        exponent: 19 (0x13)
*/
Certificate::Certificate() {}

Certificate::Certificate(std::string _subject, std::string _not_before, \
                        std::string _not_after, Key _key):
    subject(_subject),
    issuer(ISSUER),
    not_before(_not_before),
    not_after(_not_after),
    key(_key) {
}

/******************************************************************************
 * serial_num:      1byte
 * subject_len:     4 bytes be
 * subject;         string
 * not_before_len:  4 bytes be
 * not_before:      string
 * not_after_len:   4 bytes be
 * not_before:      string
 * exponent:        1 byte
 * module:          2 bytes
*/
void Certificate::send(Socket& skt) {
    skt.sendNumber(this->serial_number);
    skt.sendAll(this->subject);
    skt.sendAll(this->not_before);
    skt.sendAll(this->not_after);
    this->key.send(skt);
}


void Certificate::recive(Socket& skt) {
    skt.reciveNumber(&this->serial_number);
    skt.reciveAll(this->subject);
    skt.reciveAll(this->not_before);
    skt.reciveAll(this->not_after);
    this->key.recive(skt);
}

std::string toHexaString(int n, int len) {
    std::string hexa;
    hexa += std::string("(0x");

    std::stringstream stream;
    stream << std::hex << n;
    std::string number = stream.str();
    //2 digits per byte
    int to = len*2 - number.length();
    for (int i = 0; i < to; i++) {
        hexa += '0';
    }
    return hexa + number + ')';
}

std::string Certificate::getSubject() {
    return this->subject;
}

Key Certificate::getKey() {
    return this->key;
}

std::string Certificate::toString() {
    std::string o;
    o += std::string(CERTIFICATE);
    int n = (int) this->serial_number;
    std::string sn = toHexaString(n, SN_SIZE);
    o += SERIAL_NUMBER + std::to_string(n) + ' ' + sn + '\n';

    o += SUBJECT + this->subject + '\n'; 
    o += ISSUER;
    o += VALIDITY;
    o += NOT_BEFOR + this->not_before + '\n';
    o += NOT_AFTER + this->not_after + '\n';

    o += KEY;

    Key k = this->key;
    o += MODULE;
    k.printModule(o, toHexaString);
    o += "\n";
    o += EXPONENT;
    k.printPublicExponent(o, toHexaString);

    return o;
}

/******************************************************************************
 *0 certificate:
 *1    serial number: 1 (0x00000001)
 *2    subject: Federico Manuel Gomez Peter
 *3    issuer: Taller de programacion 1
 *4    validity:
 *5        not before: Mar 28 21:33:04 2019
 *6        not after: May 27 21:33:04 2019
 *7    subject public key info:
 *8        modulus: 253 (0x00fd)
 *9        exponent: 19 (0x13)
*/
uint32_t Certificate::send(std::string filename, Socket& skt) {
    std::ifstream file;
    file.open(filename);
    std::string line;
    int count = 0;
    int pos;
    int len;
    std::string module;
    std::string exp;
    Hash hash;
    while (std::getline(file, line, '\n')) {
        if (!file.eof()) {
            hash.load(line + '\n');
        } else {
            hash.load(line);
        }
        //std::cout << "Line: " << line << '\n';
        pos = line.find(':');
        line = line.c_str();
        len = line.length();
        if (pos + 2 > len) { //certificate:\0
            ++count;
            continue;
        }
        line = line.substr(pos + 2,len);
        if (count == 1) {
            len = line.find(' ');
            skt.sendNumber((uint32_t)std::stoi(line.substr(0, len)));
        } else if ((count == 2) | (count == 5) | (count == 6)) {
            skt.sendAll(line);
        } else if (count == 8) {
            len = line.find(' ');
            module = line.substr(0, len);
        } else if (count == 9) {
            len = line.find(' ');
            exp = line.substr(0, len);
        }        
        ++count;
    }
    Key key(exp, module);
    key.send(skt);
    return hash();
}

std::ostream& operator<<(std::ostream &o, Certificate& self) {
    std::string formal_certificate;
    formal_certificate = self.toString();
    o << formal_certificate;
    return o;   
}

Certificate::~Certificate() {}

void Certificate::addSerialNumber(uint32_t _serial_number) {
    this->serial_number = _serial_number;
}
