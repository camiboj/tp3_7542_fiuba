#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "client_certificate_info_parser.h"
#include "client_time.h"

void CertificateInfoParser::run(std::string& filename, \
                                std::string& subject, std::string& from,\
                                std::string& to) {
    std::ifstream file;
    file.open(filename);
    if (!file.good()) {
        throw std::runtime_error("Error with file while parsing certificate");
    }
    std::string line;
    int i = 0;
    while (std::getline(file, line, '\n')) {
        if (i == 0) {
            subject = line.c_str();
        } else if (i == 1) {
            from = line.c_str();
        } else if (i == 2) {
            to = line.c_str();
        }
        ++i;
    }
    if (i == 1) {
        Time tm;
        tm.toString(from);
        tm.validTo();
        tm.toString(to);
    }
}

CertificateInfoParser::CertificateInfoParser() {}
CertificateInfoParser::~CertificateInfoParser() {}
