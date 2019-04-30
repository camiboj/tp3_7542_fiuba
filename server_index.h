#ifndef SERVER_INDEX_H
#define SERVER_INDEX_H
#include <string>
#include <map>
#include <mutex>
#include "common_key.h"
#include "common_certificate.h"

class Index {
    private:
        std::string& filename;
        std::map<std::string, Key> certificates;
        void parseLine(std::string& line);
        std::mutex mutex;

    public:
        //Index();
        void write();
        void saveCertificate(Certificate& certificate);
        
        bool hasCertificate(std::string str);
        bool hasCertificate(Certificate certificate);
        Key findCertificate(Certificate cartificate);
        void eraseCertificate(Certificate certificate);
        void eraseCertificate(std::string str);

        explicit Index(std::string& filename);
        ~Index();
};

#endif
