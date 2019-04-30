#include <string>
#include <iostream>
#include "server_index.h"
#include "server_acceptor.h"


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

/******************************************************************************
 * certificate:
 *    serial number: 1 (0x00000001)
 *    subject: Federico Manuel Gomez Peter
 *    issuer: Taller de programacion 1
 *    validity:
 *        not before: Mar 28 21:33:04 2019
 *        not after: May 27 21:33:04 2019
 *    subject public key info:
 *    modulus: 253 (0x00fd)
 *    exponent: 19 (0x13)
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
	    return 1;
    }

    Socket skt;
    skt.connectWithClients(argv[1]);


    std::string index_filename(argv[3]);
    Index index(index_filename);
    
    
    //skt.acceptClient();
    Key key(argv[2]);
    Acceptor acceptor(skt, index, key);
    acceptor.start();

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "q") {
            acceptor.stop();
            break;
        }
    }    
    acceptor.join();

    
    index.write();    
    return 0;
}
