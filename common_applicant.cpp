#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "common_applicant.h"

Applicant::Applicant(std::string& filename, Key _key) : 
    key(_key) {
        std::ifstream file;
        file.open(filename); 
        std::string line;
        int i = 0;
        while (std::getline(file, line, '\n')) {
            if (i == 0) {
                this->subject = line.c_str();
            } 
            else if (i == 1) {
                this->date_from = line.c_str();
            }
            else if (i == 2) {
                this->date_to = line.c_str();
            }
        }
        if (i == 2) {
            //add 30 days
        }
    }

Applicant::~Applicant(){}
