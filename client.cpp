#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include "common_socket.h"
#include "client_new_processor.h"
#include "client_revoke_processor.h"
#define ARGUMENT_ERROR_MSSG "Error: argumentos invalidos.\n"
#define CF_SIZE 4 //certificate footprint size in bytes
#define HASH_SIZE 4


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
        return 0;
    }

    std::string mode1("new");
    std::string mode2("revoke");
    std::string mode = std::string(argv[3]);
    if (mode != mode1 && mode != mode2) {
        std::cout << ARGUMENT_ERROR_MSSG;
        return 0;
    }

    char* host = argv[1];
    char* port = argv[2];
    Socket skt(host, port, 0);
    skt.connectWithServer();
    MySocket my_socket(skt);

    if (mode == mode1) {
        std::string client_key_filename(argv[5]);
        std::string certificate_information_filename(argv[4]);
        NewProcessor np(my_socket);
        np.run(client_key_filename, certificate_information_filename);
        return 0;
    }

    if (mode == mode2) { //std::string(argv[4])
        std::string certificate_filename = std::string(argv[4]);
        std::string client_key_filname = std::string(argv[5]);
        std::string server_key_filename = std::string(argv[6]);
        RevokeProcessor rp(my_socket);
        rp.run(certificate_filename, client_key_filname, server_key_filename);
        return 0;
    }
    return 0;
}
