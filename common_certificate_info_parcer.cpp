#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "common_certificate_info_parcer.h"
#include "common_time.h"

CertificateInfoParcer::CertificateInfoParcer(std::string& filename, \
                                std::string& subject, std::string& from,\
                                std::string& to) {
    std::ifstream file;
    file.open(filename); 
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
        tm.plusMonth();
        tm.toString(to);
    }
}

CertificateInfoParcer::~CertificateInfoParcer() {}
