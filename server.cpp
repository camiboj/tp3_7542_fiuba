#include <string>
#include <iostream>
#include "common_applicant_request.h"
#include "server_index.h"

#define ERROR_CODE 1
#define COMMAND_SIZE 1
#define LEN_SIZE 4

/********************************** COMANDOS *********************************/

/******************************************************************************
 *          ./server <puerto/servicio> <claves> <índice>
*/



/********************************** ARCHIVOS *********************************/

/******************************************************************************
 * <claves>
 *   <exp_publico> <exp_privado> <modulo>
 *           => clave publ = (exp_publico, modulo)
 *           => clave priv = (exp_privado, modulo)
 * 
 *          uint8_t <exp_publico>
 *         uint8_t <exp_privado> 
 *         uint16_t <modulo>
*/

/******************************************************************************
 * <índice> ruta al archivo donde se listarán los distintos subjects egistrados 
 * en el sistema, con sus respectivas claves públicas, separados por una  nueva
 * linea.
 * <próximo número  de serie a insertar>
 * <subjects 1>; <exp_publico> <modulo>
 * 
 * Se recomienda investigar la libreria fstream, en particular el operador >>
*/


/********************************* EJECUCION *********************************/

/******************************************************************************
 * RECIBE:
 *  Comando:     - 1 byte sin signo
*/

/******************************************************************************
 *comando 0
 *
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

int main(int argc, char* argv[]) {
    if (argc != 4) {
        return ERROR_CODE;
    }

    std::string claves = std::string(argv[2]);
    std::string indice = std::string(argv[3]);
     if (argc !=4) {
	    fprintf(stderr, "Uso:\n./server <puerto> <input> [<template>]\n");
	    return 1;
    }

    
    Socket skt(argv[1]);
    skt.connectWithClients();
    skt.acceptClient();
    
    std::string index_filename(argv[3]);
    Index index(index_filename);

    uint8_t command;
    skt.reciveSome(&command, COMMAND_SIZE);
    //fprintf(stderr, "Command: %d\n", command);
    //skt.receiveSome(&command, COMMAND_SIZE);
    if (command == 0) {
        //std::cerr << "ACA \n";
        ApplicantRequest ap;
        ap.recive(skt);

        /*  
         * Recibo:
         * 
         * <subject__size>  4 bytes big endian sin signo
         * <subject>        String sin ‘\0’
         * <module>          2 bytes big endian sin signo
         * <exponent>       1 byte
         * <date__size>     4 bytes big endian sin signo
         * <date_from>      String sin ‘\0’
         * <date__size>     4 bytes big endian sin signo
         * <dat_to>         String sin ‘\0’
        */
    }

    /*
    char* port = argv[1];
    char* client_key_filename = argv[4];
    char* server_key_filename = argv[5];
    char* certificate = argv[6];
    */
    
    return 0;
}
