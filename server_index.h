#ifndef SERVER_INDEX_H
#define SERVER_INDEX_H
#include <string>
#include <map>
#include "common_key.h"
#include "common_certificate.h"

class Index {
    private:
        std::string& filename;
        std::map<std::string, Key> certificates;
        void parseLine(std::string& line);

    public:
        //Index();
        void write();
        void saveCertificate(std::string str, 
                            Key& key,
                            Certificate& certificate);
        
        bool hasCertificate(std::string str);
        void erase(std::string str);

        explicit Index(std::string& filename);
        ~Index();
};

#endif
