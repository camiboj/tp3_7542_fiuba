#ifndef SERVER_INDEX_H
#define SERVER_INDEX_H
#include <string>
#include <map>
#include "common_key.h"

class Index {
    private:
        uint32_t  serial_number;
        std::map<std::string, Key> certificates;
        void split(std::string& str, char c, std::queue<std::string>& container);
        void parseLine(std::string& line);
    public:
        //Index();
        
        Index(std::string& filename);
        ~Index();
};

#endif