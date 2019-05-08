#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ios>
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
Certificate::Certificate() {}

Certificate::Certificate(std::string& _subject, std::string& _not_before, \
                        std::string& _not_after, Key _key):
    key(_key) {
            subject = std::move(_subject);
            issuer = std::string(ISSUER);
            not_before = std::move(_not_before);
            not_after = std::move(_not_after);
}

void Certificate::send(Protocol& skt) {
    skt.send(this->serial_number);
    skt.send(this->subject);
    skt.send(this->not_before);
    skt.send(this->not_after);
    this->key.send(skt);
}


void Certificate::receive(Protocol& skt) {
    skt.receive(this->serial_number);
    skt.receive(this->subject);
    skt.receive(this->not_before);
    skt.receive(this->not_after);
    this->key.receive(skt);
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

uint32_t Certificate::send(std::string& filename, Protocol& skt) {
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
            std::string aux = line + '\n';
            hash.load(aux);
        } else {
            hash.load(line);
        }
        //// std::cout << "Line: " << line << '\n';
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
            uint32_t n = (uint32_t) std::stoi(line.substr(0, len));
            skt.send(n);
        } else if ((count == 2) | (count == 5) | (count == 6)) {
            skt.send(line);
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

void Certificate::addSerial(uint32_t _serial_number) {
    this->serial_number = _serial_number;
}
