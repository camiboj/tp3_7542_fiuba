#include "server_revoke_client_processor.h"
#include "common_certificate.h"
#include "common_rsa.h"
#include "common_hash.h"
#include <iostream>
#define HASH_ERROR_MSSG 2
#define HASH_ERROR 1
#define INVALID_CERTIFICATE_MSSG 1
#define INVALID_CERTIFICATE 1
#define OK 0
#define OK_MSSG 0


/******************************************************************************
 *comando 1
 *
 * RECIBE:
 *  - Certificado
 *  - Hash
 * 
 * 1- Validar que quien envió esta solicitud de baja sea quien dice ser.
 *      - Lee el subject 
 *          - Si no existe registrado en su mapa un usuario con ese nombre. 
 *              -envia uint8_t 1 y fin.
 *          - Si existe un subject registrado
 *              -continua        
 *      - Desencripta  la  huella  recibida,  primero con la clave  privada del
 *        servidor y  la pública del cliente almacenada en memoria. 
 *      - Compara el hash recibido con el que aplica él al certificado recibido
 *          !=- return ERROR_CODE 2 en formato 1 byte sin signo. 
 *          ==- continuo
 * 2- Elimina al solicitante de su mapa 
 * 3- Responde al cliente con un código 0
*/

RevokeClientProcessor::RevokeClientProcessor(Index& _index, Key _key): 
    index(_index), 
    server_key(_key) {}

RevokeClientProcessor::~RevokeClientProcessor() {}

int RevokeClientProcessor::run(Socket& skt) {
    Certificate certificate;
    certificate.recive(skt);
    uint32_t encryption = 0;
    skt.reciveNumber(&encryption);
    //std::cout << "Encrypt: " << encryption << '\n';
    uint8_t answer;
    if (!this->index.hasCertificate(certificate)) {
        answer = INVALID_CERTIFICATE_MSSG;
        skt.sendNumber(answer);
        return INVALID_CERTIFICATE;
    }
    Rsa rsa(index.findCertificate(certificate), server_key);
    uint32_t desencrytion = rsa.privateDesencryption(encryption);
    uint32_t client_hash = rsa.publicDesencryption(desencrytion);
    std::cout << certificate.toString() << '\n';
    Hash hash(certificate.toString());
    uint32_t my_hash = hash();
    if (my_hash != client_hash) {
        answer = HASH_ERROR_MSSG;
        skt.sendNumber(answer);
        return HASH_ERROR;
    }
    index.eraseCertificate(certificate);
    answer = OK_MSSG;
    skt.sendNumber(answer);
    return OK;
}
