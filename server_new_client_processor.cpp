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

NewClientProcessor::NewClientProcessor(MySocket* _skt, \
                                        Index& _index, Key _key) : 
    skt(_skt),
    index(_index), 
    server_key(_key),
    is_dead(false) {}

NewClientProcessor::~NewClientProcessor() {
    delete this->skt;
}

void NewClientProcessor::receiveInfo() {
    skt->receiveAll(this->subject);
    this->client_key.receive(*skt);
    
    skt->receiveAll(this->date_from);

    skt->receiveAll(this->date_to);
    if (subject.size() == 0) {
        throw std::runtime_error("");
    }
}

std::string NewClientProcessor::createCertificate() {
    Certificate certificate(this->subject, this->date_from, this->date_to,\
             this->client_key);
    this->index.saveCertificate(certificate);
    std::string result = certificate.toString();
    certificate.send(*skt);
    return result;
}

bool NewClientProcessor::checkCertificate() {
    uint8_t answer = CERTIFICATE_OK;
    if (index.hasCertificate(this->subject)) { 
        answer = CERTIFICATE_ERROR;
        skt->sendNumber(&answer);
        //trow algo;
        return false;
    } 
    skt->sendNumber(&answer);
    return true;
}

uint32_t encrypt(Key client_key, Key server_key, uint32_t hash) {
    Rsa rsa(client_key, server_key);
    uint32_t encryption = rsa.privateEncryption(hash);
    encryption = rsa.publicEncryption(encryption);
    return encryption;
}

void NewClientProcessor::run() {
    //std::string str;
    //    this->skt->receiveAll(str);
    //    std::cout << "STR: " << str << '\n';
    try {
        this->receiveInfo();
    }
    catch (...) {
        this->is_dead = true;
        return;
    }
    bool s = this->checkCertificate();
    if (!s) return;
    std::string formal_certificate = this->createCertificate();
    
    Hash hash(formal_certificate);
    uint32_t hashed_certificate =  hash();
    uint32_t encryption = encrypt(this->client_key, this->server_key,\
                                 hashed_certificate);

    skt->sendNumber(&hashed_certificate);
    skt->sendNumber(&encryption);
    
    uint8_t hash_status = 0;
    skt->receiveNumber(&hash_status);
    if (hash_status == HASH_ERROR) {
        index.eraseCertificate(this->subject);
    }
    
    this->is_dead = true;
    return;
}

bool NewClientProcessor::isDead() {
    return this->is_dead;
}

void NewClientProcessor::stop() {
    this->skt->stop();
}
