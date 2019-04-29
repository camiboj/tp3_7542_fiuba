
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include "common_certificate.h"
#include "common_socket.h"
#include "client_applicant_request.h"
#include "common_hash.h"
#include "common_rsa.h"
#define ARGUMENT_ERROR_MSSG "Error: argumentos invalidos.\n"
#define ARGUMENT_ERROR 0
#define USER_ERROR_MSSG "Error: usuario no registrado.\n"
#define USER_ERROR 0
#define CERTIFICATE_ERROR_MSSG "Error: ya existe un certificado.\n"
#define CERTIFICATE_ERROR 0
#define CERIFICATE_ERROR_RECIVED_MSSG 0
#define HASH_ERROR_MSSG "Error: los hashes no coinciden.\n"
#define HASH_ERROR 0
#define HASH_ERROR_SERVER_MSSG 1
#define HASH_OK_SERVER_MSSG 0
#define CF_SIZE 4 //certificate footprint size in bytes
#define HASH_SIZE 4
#define NOTIFICATION_SIZE 1
#define HASH_ERROR_SM 2
#define INVALID_CERTIFICATE 1
#define INVALID_CERTIFICATE_MSSG "Error: usuario no registrado.\n"
#define CERT_FP "Huella del servidor: "
#define SH "Hash del servidor: "
#define MH "Hash calculado: "
#define HASH "Hash calculado: "
#define PRIVATE_ENCRYPTION "Hash encriptado con la clave privada: "
#define PUBLIC_ENCRYPTION "Huella enviada: "

/********************************** COMANDOS *********************************/

/******************************************************************************
 *new
 *
 *   ./client <ip/hostname> <puerto/servicio> new <información certificado>
 *           <claves clientes> <pública servidor>
*/

/******************************************************************************
 *revoke
 *
 *   ./client <ip/hostname> <puerto/servicio> revoke <certificado> 
 *           <claves clientes> <pública servidor> 
*/



/********************************** ARCHIVOS *********************************/

/******************************************************************************
 *both
 * <claves clientes>
 *   <exp_publico> <exp_privado> <modulo>
 *           => clave publ = (exp_publico, modulo)
 *           => clave priv = (exp_privado, modulo)
 *
 * <pública servidor>
 *   <exp_publico> <modulo>
 *
 * Los distintos campos pueden estar separados por 1 o más espacios, únicamente
 * se garantiza que estos se encuentran en una misma línea.
*/

 /******************************************************************************
 *new
 * <información certificado> 
 *   Federico Manuel Gomez Peter         <---subject
 *   Mar 28 21:33:04 2019                MMM DD HH:mm:SS YYYY
 *   May 27 21:33:04 2019                MMM DD HH:mm:SS YYYY
*/

/********************************* EJECUCION ********************************/

/******************************************************************************
 *new
 *
 * ENVIAR:
 * 
 *  - Comando:     - 1 byte sin signo (0)
 *
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
 * ANSWER: ​
 *  0 -> subject ya tiene un certificado vigente => CERTIFICATE_ERROR_MSSG
 *  1 ​-> El certificado creado por el servidor.
 *    -> El hash del certificado, encriptado con la clave privada del servidor
 *       y luego encriptado con la clave pública del cliente.
 *
 * EJECUTAR:
 * if (recibi 1 + datos):
 *   1- Desencriptar el  hash, primero usando su propia clave  privada, y luego
 *      usando la clave pública del servidor. 
 *   2- Calcula el hash del certificado recibido 
 *   3- if (cálculo coincide con el del que recibió del servidor). 
 *       3.T- Guarda el certificado en un archivo
 *            Notifica al servidor su correcta recepción (enviar uint8_t 0)
 *            Termina su ejecución normalmente.
 *       3.F- Notifica al servidor la falla (enviar uint8_t 1)​ , 
 *            Cierra su ejecución escribiendo en pantalla HASH_ERROR_MSSG
*/

/******************************************************************************
 *revoke
 * 
 * 
 * ENVIAR:
 * 
 *  - Comando:     - 1 byte sin signo (1)
 * 
 *  - Certificado: - Aún no se pero sin los rotulos "subject:" y esos.
 * 
 *  - Hash:        - Encriptado primero con la clave privada del cliente y luego
 *                   encriptado con la pública del servidor.
 * 
 * 
 * ANSWER:​ (1 byte sin signo)
 *  0-> todo piola
 *  1-> si no existe registrado en el mapa del server un usuario con ese nombre
 *  2-> si los hashes no cinciden 
 * 
 * 
 * EJECUTAR:
 *  1- imprime por salida estándar el hash calculado 
 *  2- imprime por salida estándarla huella enviada al servidor
 *     huella del certificado -> entero de 4 bytes sin signo en big endian
*/

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cout << ARGUMENT_ERROR_MSSG;
        return ARGUMENT_ERROR;
    }
    std::string mode1("new");
    std::string mode2("revoke");
    std::string mode = std::string(argv[3]);
    if (mode != mode1 && mode != mode2) {
        std::cout << ARGUMENT_ERROR_MSSG;
        return ARGUMENT_ERROR;
    }

    char* host = argv[1];
    char* port = argv[2];
    Socket skt(host, port, 0);
    skt.connectWithServer();

    

    if (mode == mode1) {
        std::string client_key_filename(argv[5]);
        std::string certificate_information_filename(argv[4]);

        ApplicantRequest request(certificate_information_filename,\
                                 client_key_filename);
        request.send(skt);
        uint8_t answer = 1;
        skt.reciveNumber(&answer);
        if (answer == CERIFICATE_ERROR_RECIVED_MSSG) {
            std::cout << CERTIFICATE_ERROR_MSSG;
            return CERTIFICATE_ERROR;
        }

        Certificate certificate;
        certificate.recive(skt);
        std::string formal_certificate;
        formal_certificate = certificate.toString();


        Hash hash(formal_certificate);
        uint32_t my_hash =  hash();
        

        uint32_t server_hash = 0;
        skt.reciveNumber(&server_hash);
        
        uint32_t certificate_footprint = 0;
        skt.reciveNumber(&certificate_footprint);

        uint8_t notification = HASH_OK_SERVER_MSSG;
        if (my_hash != server_hash) {
            notification = HASH_ERROR_SERVER_MSSG;
            skt.sendNumber(notification);
            std::cout << HASH_ERROR_MSSG;
            return HASH_ERROR;
        }
        skt.sendNumber(notification);
        std::cout << CERT_FP << certificate_footprint << '\n';
        std::cout << SH << server_hash << '\n';
        std::cout << MH << my_hash << '\n';

        std::string filename = request.getSubject() + ".cert";
        std::ofstream file;
        file.open(filename, std::ofstream::out | std::ofstream::trunc);
    
        if (!file.is_open()) {
            //exc
        }
        file << formal_certificate;
        return 0;
    }
    if (mode == mode2) {
        uint8_t command = 1;
        skt.sendNumber(command);
        Certificate certificate;
        uint32_t hash = certificate.send(std::string(argv[4]), skt);
        Key server_key(argv[6]);
        Key client_key(argv[5]);
        Rsa rsa(server_key, client_key);
        uint32_t priv_encryption = rsa.privateEncryption(hash);
        uint32_t publ_encryption = rsa.publicEncryption(priv_encryption);
        skt.sendNumber(publ_encryption);
        uint8_t status = 0;
        skt.reciveNumber(&status);
        if (status == HASH_ERROR_SM) {
            std::cout << HASH_ERROR_MSSG;
            return 0;
        }
        if (status == INVALID_CERTIFICATE) {
            std::cout << INVALID_CERTIFICATE_MSSG;
            return 0;
        }
        std::cout << HASH << (int) hash << '\n';
        std::cout << PRIVATE_ENCRYPTION << (int) priv_encryption << '\n';
        std::cout << PUBLIC_ENCRYPTION << (int) publ_encryption << '\n';
    }
    return 0;
}
