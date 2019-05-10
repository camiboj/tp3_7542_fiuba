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
        uint32_t serial_number;

        

    public:
        Index();
        void write();
        void save(Certificate& certificate);
        //void increaseSerialNumber();

        /*
        * Vuelve a almacenar el certificado pasado por parametro.
        * Con client_key valor.
        */
        void putBack(Certificate& certificate,  Key& client_key);
        
        
        Key find(Certificate& cartificate);

        /* Borra el certificado
        *  Si el certificado no existe lanza una exepcion de tipo
        *  InexistentCertificate
        */
        void erase(Certificate& certificate);
        void erase(std::string& str);

        explicit Index(std::string& filename);
        ~Index();


        bool has(std::string& str);
        bool has(Certificate& certificate);
};

#endif
