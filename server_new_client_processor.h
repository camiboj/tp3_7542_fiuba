#ifndef SERVER_NEW_CLIENT_PROCESSOR_H
#define SERVER_NEW_CLIENT_PROCESSOR_H
#include <stdint.h>
#include <string>
#include "common_socket.h"
#include "common_key.h"
#include "common_string.h"
#include "server_index.h"

class NewClientProcessor {
    private:
        std::string subject;
        Key client_key;
        std::string date_from;
        std::string date_to;
        Index& index;
        Key server_key;
        void reciveInfo(Socket& skt);
        std::string createCertificate(Socket& skt);

    public:
        /*
        * Recibe los dos archivos necesarios para solicitar un nuevo aplicante
        */
        NewClientProcessor(Index& _index, Key key);
        
        ~NewClientProcessor();
        /*  
         * Envia a traves del socket recibido por parametro una solucitud con
         * formato:
         * 
         * <comando>        1 byte con valor 0.
         * <subject__size>  4 bytes big endian sin signo
         * <subject>        String sin ‘\0’
         * <modul>          2 bytes big endian sin signo
         * <exponent>       1 byte
         * <date__size>     4 bytes big endian sin signo
         * <date_from>      String sin ‘\0’
         * <date__size>     4 bytes big endian sin signo
         * <dat_to>         String sin ‘\0’
        */
        int run(Socket& skt);
        bool checkCertificate(Socket& skt);
};

#endif
