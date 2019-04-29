#ifndef SERVER_REVOKE_CLIENT_PROCESSOR_H
#define SERVER_REVOKE_CLIENT_PROCESSOR_H
#include <stdint.h>
#include <string>
#include "common_socket.h"
#include "common_key.h"
#include "common_string.h"
#include "server_index.h"

class RevokeClientProcessor {
    private:
        Index& index;
        Key server_key;
    public:
        RevokeClientProcessor(Index& _index, Key key);
        
        ~RevokeClientProcessor();
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
        //bool checkCertificate(Socket& skt);
};

#endif
