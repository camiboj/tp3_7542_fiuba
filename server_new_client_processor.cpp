#include <string>
#include <iostream>
#include "server_new_client_processor.h"
#include "common_certificate.h"
#include "common_hash.h"
#include "common_rsa.h"
#define ENCRYPTION_SIZE 4
#define HASH_ERROR 1
#define CERTIFICATE_ERROR 0
#define CERTIFICATE_OK 1
#define CERT_STATUS_SIZE 1
#define ERROR_CODE 1
#define HASH_STATUS_SIZE 1
/******************************************************************************
 * RECIBE:
 *  - Subject:     - Longitud: 4 bytes big endian sin signo.
 *                 - String sin ‘\0’
 *
 *  - Módulo:      - 2 bytes en big endian sin signo
 * 
 *  - Exponente:   - 1 byte
 *
 *  - Fechas:      - Longitud: 4 bytes big endian sin signo.
 *                 - String sin ‘\0’
 * 
 * 
 * ANSWER (1 byte, sin signo)
 *  0 -> ​si el subject ya tiene un certificado vigente
 *  1 ->​ en caso contrario.
 *       
 * if(1)
 *  1- El certificado se crea correctamente, 
 *  2- Almacena en memoria el subject y la clave pública
 *  3- Envia el certificado creado por el servidor.
 *  4- Envia el hash del certificado, encriptado con la clave privada del 
 *     servidor y luego encriptado con la clave pública del cliente
 *  5- Espera 1 byte sin signo​ del cliente
 *      0 -> todo ok chau.
 *      1 -> elimina de su almacenamiento interno los datos del certificado
 * 
*/

NewClientProcessor::NewClientProcessor(Index& _index, Key key) : 
    index(_index), 
    server_key(key) {
}

NewClientProcessor::~NewClientProcessor() {}

void NewClientProcessor::reciveInfo(Socket& skt) {
    String my_subject(this->subject);
    my_subject.recive(skt);

    this->client_key.recive(skt);
    
    String my_date_from(this->date_from);
    my_date_from.recive(skt);

    String my_date_to(this->date_to);
    my_date_to.recive(skt);
}

std::string NewClientProcessor::createCertificate(Socket& skt) {
    Certificate certificate(this->subject, this->date_from, this->date_to,\
             this->client_key);
    this->index.saveCertificate(this->subject, this->client_key, certificate);
    std::string result = certificate.toString();
    certificate.send(skt);
    return result;
}

bool NewClientProcessor::checkCertificate(Socket& skt) {
    uint8_t answer = CERTIFICATE_OK;
    if (index.hasCertificate(this->subject)) {
        answer = CERTIFICATE_ERROR;
        skt.sendAll(&answer, CERT_STATUS_SIZE);
        //trow algo;
        return false;
    } 
    skt.sendAll(&answer, 1);
    return true;
}

uint32_t encrypt(Key client_key, Key server_key, uint32_t hash) {
    Rsa rsa(client_key, server_key);
    uint32_t encryption = rsa.privateEncryption(hash);
    encryption = rsa.publicEncryption(encryption);
    return encryption;
}

int NewClientProcessor::run(Socket& skt) {
    this->reciveInfo(skt);
    bool s = this->checkCertificate(skt);
    if (!s) return ERROR_CODE;
    std::string formal_certificate = this->createCertificate(skt);
    
    Hash hash(formal_certificate);
    uint32_t hashed_certificate =  hash();
    uint32_t encryption = encrypt(this->client_key, this->server_key,\
                                 hashed_certificate);

    hashed_certificate = htobe32(hashed_certificate);
    skt.sendAll(&hashed_certificate, ENCRYPTION_SIZE);
    
    encryption = htobe32(encryption);
    skt.sendAll(&encryption, ENCRYPTION_SIZE);

    uint8_t hash_status;
    skt.reciveAll(&hash_status, HASH_STATUS_SIZE);
    if (hash_status == HASH_ERROR) {
        index.erase(this->subject);
    }
    return ERROR_CODE;
}
